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
  geode::log::debug("amogus");
  m_toolbar = $verifyPtr(Toolbar::create());
  addChild(m_toolbar);

  m_screensContainer->setContentSize(m_screensContainer->getContentSize());

  m_screensContainer->addListener<frameworks::NodeLayoutUpdated>([](frameworks::NodeLayoutUpdated*){
    geode::log::debug("[OsuGame->m_screensContainer]: ");
    return true;
  });

  return true;
}

bool OsuGame::doDispatchEvent(frameworks::Event* event, std::type_index type) {
  if (!Game::doDispatchEvent(event, type)) return false;
  if (type == typeid(frameworks::NodeLayoutUpdated)) {
    m_screensContainer->setContentSize({getContentWidth(), getContentHeight()-(m_toolbar->isOpen() ? ToolbarConstants::c_height : 0)});
  }
  return true;
}

void OsuGame::showToolbar() {
  m_toolbar->show();
  m_screensContainer->runAction(easingsActions::CCEaseOut::create(
    CCCustomTween::create(getContentHeight(), getContentHeight()-ToolbarConstants::c_height, 0.5, this, [](float gaming){}), 5
  ));
};
void OsuGame::hideToolbar() {
  m_toolbar->hide();
  m_screensContainer->runAction(easingsActions::CCEaseOut::create(
    CCResizeTo::create(0.5,getContentWidth(),getContentHeight()), 5
  ));
};

GDL_NS_END
