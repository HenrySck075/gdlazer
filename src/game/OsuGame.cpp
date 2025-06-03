#include "OsuGame.hpp"
#include "overlays/toolbar/ToolbarConstants.hpp"
#include "../frameworks/graphics/CCEase2.hpp"
#include "../helpers/CustomActions.hpp"

namespace {
  std::mutex g_fish;
};
GDL_NS_START
geode::Ref<OsuGame> OsuGame::get(bool create) {
  auto s_instance = Game::get();
  if ((!s_instance || geode::cast::typeinfo_cast<OsuGame*>(s_instance.operator->()) == nullptr) && create) {
    g_fish.lock();
    s_instance = new OsuGame();
    if (s_instance->init()) {
      s_instance->autorelease();
      setInstance(s_instance);
    } else {
      delete s_instance;
      s_instance = nullptr;
    }
    g_fish.unlock();
  }
  return static_cast<OsuGame*>(s_instance.operator->());
};

bool OsuGame::init() {
  if (!Game::init()) return false;
  m_toolbar = $verifyPtr(Toolbar::create());
  addChild(m_toolbar);
  
  addChild(m_cursor = frameworks::CCResizableSprite::create("menu-cursor.png"_spr), 99999);
  m_cursor->setContentSize({10,442/(312/10.f)});
  m_cursor->setAnchorPoint({0,1});

  m_everypence->setContentSize(m_everypence->getContentSize());

  return true;
}

bool OsuGame::doDispatchEvent(frameworks::Event* event, std::type_index type) {
  if (
    type == typeid(frameworks::MouseEvent)
  ) {
    auto cure = static_cast<frameworks::MouseEvent*>(event);
    if (cure->m_eventType == frameworks::MouseEventType::Move) {
      m_cursor->setPosition({cure->m_position.x, cure->m_position.y});
    }
  }
  /// Relaunch on Ctrl-R
  #ifdef GDL_DEBUG
  if (type == typeid(frameworks::KeyEvent)) {
    auto ke = static_cast<frameworks::KeyEvent*>(event);
    if (ke->m_pressed && ke->m_key == cocos2d::KEY_R && ke->m_modifiers.ctrl) {
      geode::utils::game::restart();
      return true;
    }
  }
  #endif
  if (!Game::doDispatchEvent(event, type)) return false;
  if (type == typeid(frameworks::NodeLayoutUpdated)) {
    m_everypence->setContentSize({getContentWidth(), getContentHeight()-(m_toolbar->isOpen() ? ToolbarConstants::c_height : 0)});
  }
  return true;
}

bool OsuGame::doDEMidhook(frameworks::Event* event, std::type_index type) {
  bool ret = m_toolbar->doDispatchEvent(event, type);

  return type == typeid(frameworks::NodeLayoutUpdated) || ret;
}

void OsuGame::showToolbar() {
  m_toolbar->show();
  m_everypence->runAction(easingsActions::CCEaseOut::create(
    CCCustomTween::create(
      getContentHeight(), 
      getContentHeight()-m_everypence->processUnit(
        ToolbarConstants::c_height, 
        frameworks::Unit::UIKit, false
      ), 
      0.5, 
      this, customtween_selector(OsuGame::setMainContainerHeight)
    ), 5
  ));
};
void OsuGame::hideToolbar() {
  m_toolbar->hide();
  m_everypence->runAction(easingsActions::CCEaseOut::create(
    CCCustomTween::create(
      getContentHeight()-m_everypence->processUnit(
        ToolbarConstants::c_height, 
        frameworks::Unit::UIKit, false
      ), 
      getContentHeight(), 
      0.5, 
      this, customtween_selector(OsuGame::setMainContainerHeight)
    ), 5
  ));
};

void OsuGame::setMainContainerHeight(float height) {
  m_everypence->setContentSize({100, height}, frameworks::Unit::Percent, frameworks::Unit::OpenGL);
  m_everypence->dispatchEvent(new frameworks::NodeLayoutUpdated(m_everypence)); // why
}

GDL_NS_END
