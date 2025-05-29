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

  m_screensContainer->setContentSize(m_screensContainer->getContentSize());

  return true;
}

bool OsuGame::doDispatchEvent(frameworks::Event* event, std::type_index type) {
  if (!Game::doDispatchEvent(event, type)) return false;
  if (type == typeid(frameworks::NodeLayoutUpdated)) {
    m_screensContainer->setContentSize({getContentWidth(), getContentHeight()-(m_toolbar->isOpen() ? ToolbarConstants::c_height : 0)});
  }
  return true;
}

bool OsuGame::doDEMidhook(frameworks::Event* event, std::type_index type) {
  bool ret = m_toolbar->doDispatchEvent(event, type);

  return type == typeid(frameworks::NodeLayoutUpdated) || ret;
}

void OsuGame::showToolbar() {
  m_toolbar->show();
  m_screensContainer->runAction(easingsActions::CCEaseOut::create(
    CCCustomTween::create(
      getContentHeight(), 
      getContentHeight()-m_screensContainer->processUnit(
        ToolbarConstants::c_height, 
        frameworks::Unit::UIKit, false
      ), 
      0.5, 
      this, customtween_selector(OsuGame::setScreensContainerHeight)
    ), 5
  ));
};
void OsuGame::hideToolbar() {
  m_toolbar->hide();
  m_screensContainer->runAction(easingsActions::CCEaseOut::create(
    CCCustomTween::create(
      getContentHeight()-m_screensContainer->processUnit(
        ToolbarConstants::c_height, 
        frameworks::Unit::UIKit, false
      ), 
      getContentHeight(), 
      0.5, 
      this, customtween_selector(OsuGame::setScreensContainerHeight)
    ), 5
  ));
};

void OsuGame::setScreensContainerHeight(float height) {
  m_screensContainer->setContentSize({100, height}, frameworks::Unit::Percent, frameworks::Unit::OpenGL);
  m_screensContainer->dispatchEvent(new frameworks::NodeLayoutUpdated(m_screensContainer)); // why
}

GDL_NS_END
