#include "Game.hpp"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"
#include "audio/AudioManager.hpp"
#include "input/events/KeyEvent.hpp"
#include <mutex>
#include "graphics/containers/Container.hpp"

GDF_NS_START

static geode::Ref<Game> s_instance;

bool Game::init() {
  if (!CCScene::init()) return false;

  scheduleUpdate();

  /*TEST CODE ~ Remove after finished*/

  m_screensContainer = Container::create();
  m_screensContainer->setContentSize({100,100}, Unit::Percent);
  m_overlaysContainer = Container::create();
  m_overlaysContainer->setContentSize({100,100}, Unit::Percent);

  addChild(m_screensContainer);
  addChild(m_overlaysContainer);

  return true;
};

static CCSize s_lastWinSize = {0,0};

void Game::update(float dt) {
  CCNode::update(dt);
  for (auto screen : m_invisibleQueue) {
    if (screen->numberOfRunningActions() == 0) {
      screen->setVisible(false);
      m_invisibleQueue.inner()->removeObject(screen);
    }
  }
  decltype(m_removalQueue) rqRemove;
  for (auto screen : m_removalQueue) {
    if (screen->numberOfRunningActions() == 0) {
      rqRemove.push_back(screen);
      screen->removeFromParentAndCleanup(true);
    }
  }
  for (auto screen : rqRemove) {
    m_removalQueue.inner()->removeObject(screen);
  }
  decltype(m_overlayRemovalQueue) orqRemove;
  for (auto overlay : m_overlayRemovalQueue) {
    if (overlay->numberOfRunningActions() == 0) {
      orqRemove.push_back(overlay);
      overlay->removeFromParentAndCleanup(true);
    }
  }
  for (auto overlay : orqRemove) {
    m_overlayRemovalQueue.inner()->removeObject(overlay);
  }

  AudioManager::get()->update(dt);

  /// heavy workaround because glfw win size hooks stopped working
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  if (s_lastWinSize != winSize) {
    if (s_lastWinSize != CCSize{0,0}) {
      setContentSize(winSize);
      dispatchEvent(new NodeLayoutUpdated(this));
    }
    s_lastWinSize = winSize;
  }
}

void Game::pushScreen(Screen* screen) {
  m_screensContainer->addChild(screen);
  screen->setContentSize({100,100}, Unit::Percent);
  ScreenTransitionEvent e{m_currentScreen, screen};
  screen->onScreenEnter(e);
  if (m_currentScreen) {
    m_currentScreen->onScreenExit(e);
    m_invisibleQueue.push_back(m_currentScreen);
  }
  m_screenStack.push_back(screen);
  m_currentScreen = screen;
}

void Game::replaceScreen(Screen* screen) {
  m_screensContainer->addChild(screen);
  screen->setContentSize({100,100}, Unit::Percent);
  ScreenTransitionEvent e{m_currentScreen, screen};
  screen->onScreenEnter(e);
  if (m_currentScreen) {
    m_currentScreen->onScreenExit(e);
    m_screenStack.pop_back();
    m_removalQueue.push_back(m_currentScreen);
  }
  m_screenStack.push_back(screen);
  m_currentScreen = screen;
}

Screen* Game::popScreen() {
  if (m_screenStack.size() == 0) return nullptr;
  geode::Ref<Screen> screen = m_screenStack.pop_back();
  ScreenTransitionEvent e{m_currentScreen, screen};
  if (screen) screen->onScreenEnter(e);
  if (m_currentScreen) {
    m_currentScreen->onScreenExit(e);
    m_removalQueue.push_back(m_currentScreen);
  }
  if (m_screenStack.size() > 0) {
    m_currentScreen = static_cast<Screen*>(m_screenStack.inner()->lastObject());
  } else {
    m_currentScreen = nullptr;
  }
  return screen;
}

void Game::pushOverlay(OverlayContainer* overlay) {
  if (!overlay->m_shown) return overlay->show();
  m_overlaysContainer->addChild(overlay);
  m_currentOverlay = overlay;
  m_overlayStack.push_back(overlay);
};
void Game::popOverlay(OverlayContainer* overlay) {
  if (overlay->m_shown) return overlay->hide();
  if (m_overlayStack.size() > 0) {
    m_overlayRemovalQueue.push_back(overlay);
    m_overlayStack.inner()->removeObject(overlay);
    if (m_overlayStack.size() > 0) {
      m_currentOverlay = static_cast<OverlayContainer*>(m_overlayStack.inner()->lastObject());
    } else {
      m_currentOverlay = nullptr;
    }
  }
};

std::mutex g_fish;
void Game::setInstance(geode::Ref<Game> instance) {
  s_instance = instance;
};

geode::Ref<Game> Game::get(bool create) {
  if (!s_instance && create) {
    g_fish.lock();
    s_instance = new Game();
    if (s_instance->init()) {
      s_instance->autorelease();
    } else {
      delete s_instance;
      s_instance = nullptr;
    }
    g_fish.unlock();
  }
  return s_instance;
};
bool Game::doDispatchEvent(Event* event, std::type_index type) {
  geode::Ref<Event> eventRefHolder(event);
  // Handle the event
  if (!EventTarget::doDispatchEvent(event, type)) {
    return false;
  }

  // Propagate to children
  if (event->m_propagateStopped) {
    return true;
  }
  if (!doDEMidhook(event, type)) return false;
  if (type == typeid(NodeLayoutUpdated)) {
    m_screensContainer->doDispatchEvent(event, type);
    m_overlaysContainer->doDispatchEvent(event, type);
    return true;
  }
  if (m_currentOverlay) m_currentOverlay->doDispatchEvent(event, type);
  else m_currentScreen->doDispatchEvent(event, type);
  return true;
};
GDF_NS_END 
