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
  addListener<KeyEvent>([](KeyEvent* e){
    if (e->m_key == cocos2d::KEY_Escape && e->m_pressed) {
      cocos2d::CCDirector::get()->popScene();
    }
    return true;
  });

  m_screensContainer = Container::create();
  m_screensContainer->setContentSize({100,100}, Unit::Percent);

  this->addChild(m_screensContainer);

  return true;
};

void Game::update(float dt) {
  CCNode::update(dt);
  for (auto screen : m_invisibleQueue) {
    if (screen->numberOfRunningActions() == 0) {
      screen->setVisible(false);
      m_invisibleQueue.inner()->removeObject(screen);
    }
  }
  for (auto screen : m_removalQueue) {
    if (screen->numberOfRunningActions() == 0) {
      screen->removeFromParentAndCleanup(true);
      m_removalQueue.inner()->removeObject(screen);
    }
  }

  AudioManager::get()->update(dt);
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

Screen* Game::popScreen() {
  if (m_screenStack.size() == 0) return nullptr;
  auto screen = m_screenStack.pop_back();
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

std::mutex g_fish;
void setInstance(geode::Ref<Game> instance) {
  s_instance = instance;
};

geode::Ref<Game> Game::get(bool create) {
  if (!s_instance && create) {
    g_fish.lock();
    s_instance = new Game();
    s_instance->autorelease();
    s_instance->init();
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
  if (m_currentOverlay) m_currentOverlay->doDispatchEvent(event, type);
  else m_currentScreen->doDispatchEvent(event, type);
  return true;
};
GDF_NS_END 
