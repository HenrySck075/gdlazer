#include "ModsOverlay.hpp"
#include "geodemods/ModListContainer.hpp"
#include "../graphics/containers/TabContainer.hpp"

#include <Geode/modify/CCTransitionScene.hpp>
struct owo : public geode::Modify<owo, CCTransitionScene> {
  struct Fields {
    CCScene* m_inScene;
    CCScene* m_outScene;
  };
  virtual bool initWithDuration(float t,CCScene* scene) {
    if (!CCTransitionScene::initWithDuration(t, scene)) return false;
    auto fields = m_fields.self();
    fields->m_inScene = m_pInScene;
    fields->m_outScene = m_pOutScene;
    log::debug("[hook: CCTransitionScene]: in: {}, out: {}", m_pInScene, m_pOutScene);
    return true;
  };
};

static geode::Ref<CCLayer> s_godeLayer;

static bool s_replaceSceneHookEnabled = false;
static bool s_getWinSizeHookEnabled = false;

#include <Geode/modify/CCDirector.hpp>
struct theresmultipleccdirectorhooksscatteredaround 
  : public Modify<theresmultipleccdirectorhooksscatteredaround, CCDirector> {
    /*
  static void onModify(auto& self) {
    auto rs = self.getHook("cocos2d::CCDirector::replaceScene");
    if (!rs) {
      log::error("dum bass");
      return;
    }
    s_replaceSceneHook = rs.unwrap();
    s_replaceSceneHook->setAutoEnable(false);
    s_replaceSceneHook->disable();

    auto gws = self.getHook("cocos2d::CCDirector::getWinSize");
    if (!gws) {
      log::error("dum bass");
      return;
    }
    s_getWinSizeHook = gws.unwrap();
    s_getWinSizeHook->setAutoEnable(false);
    s_getWinSizeHook->disable();
  }
    */
  bool replaceScene(CCScene* scene) {
    auto transition = typeinfo_cast<CCTransitionScene*>(scene);
    if (s_replaceSceneHookEnabled && transition) {
      if (auto modsLayer = static_cast<owo*>(transition)->m_fields->m_inScene->getChildByID("ModsLayer")) {
        s_godeLayer = static_cast<CCLayer*>(modsLayer);
        return true;
      }
    }
    return CCDirector::replaceScene(scene);
  }
  CCSize getWinSize() {
    auto ret = CCDirector::getWinSize();
    if (!s_getWinSizeHookEnabled) return ret;
    auto game = gdlazer::game::OsuGame::get(false);
    log::debug("hi");
    if (game && s_replaceSceneHookEnabled) {
      log::debug("hello");
      ret.height -= game->processUnit(ToolbarConstants::c_height, gdlazer::frameworks::Unit::UIKit, false);
    }
    return ret;
  }
};

GDL_NS_START
using namespace frameworks;

bool ModsOverlay::init() {
  if (!WaveContainer::init(
    OverlayColorScheme::Purple
  )) return false;

  s_getWinSizeHookEnabled = true;
  s_replaceSceneHookEnabled = true;

  static_cast<CCMenuItem*>(GameManager::get()->m_menuLayer->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button"))->activate();
  
  s_replaceSceneHookEnabled = false;
  s_getWinSizeHookEnabled = false;
  /*
  auto tabContainer = $verifyPtr(TabContainer::create({
    createInstalledTab(),
    createPopularTab()
  }, m_provider->Color0()));
  */
  m_main->addChild(s_godeLayer);
  static_cast<CCMenuItemSpriteExtra*>(getChildByIdTree(s_godeLayer, {"back-menu", "back-button"}))->setTarget(this, menu_selector(ModsOverlay::onGoogooGaga));
  m_main->setContentWidth(100, Unit::Percent);
  m_main->setPadding({4, 0});
  setName("Geode");
  setTitle("Geode Mods Listing");

  m_useContainerSizeForWaves = true;

  /*addChild(*/m_bg = SwelvyBG::create()/*)*/;
  m_bg->setZOrder(m_main->getZOrder()-1);
  
  addListener<NodeSizeUpdated>([this](NodeSizeUpdated*) {
    m_bg->setContentSize(getContentSize());
    return true;
  });
  return true;
}

Container* ModsOverlay::createInstalledTab() {
  auto ret = ModListContainer::create(m_provider, InstalledModList::get(s_godeLayer));
  ret->setName("Installed");
  return ret;
}

Container* ModsOverlay::createPopularTab() {
  auto ret = Container::create();
  ret->setName("Popular");
  return ret;
}

void ModsOverlay::onOpen() {
  WaveContainer::onOpen();
  float h = getContentHeight();
  m_bg->setPositionY(m_main->getPositionY());
  m_bg->runAction(CCEaseSineOut::create(CCMoveTo::create(0.8f, {0,h})));
}

void ModsOverlay::onGoogooGaga(CCObject*) {
  hide();
}

void ModsOverlay::onClose() {
  WaveContainer::onClose();
  float h = getContentHeight();
  m_bg->runAction(CCEaseSineIn::create(CCMoveTo::create(0.5f, {0,m_pos4-15})));
}
GDL_NS_END
