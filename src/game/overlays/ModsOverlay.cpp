#include "ModsOverlay.hpp"
#include "../utils/LayerCapture.hpp"

GDL_NS_START
using namespace frameworks;

bool ModsOverlay::init() {
  if (!WaveContainer::init(
    OverlayColorScheme::Purple
  )) return false;

  auto godeLayer = captureIncomingScene([]{
    static_cast<CCMenuItem*>(GameManager::get()->m_menuLayer->getChildByID("bottom-menu")->getChildByID("geode.loader/geode-button"))->activate();
  });
  
  /*
  auto tabContainer = $verifyPtr(TabContainer::create({
    createInstalledTab(),
    createPopularTab()
  }, m_provider->Color0()));
  */
  m_main->addChild(godeLayer);
  static_cast<CCMenuItemSpriteExtra*>(getChildByIdTree(godeLayer, {"ModsLayer", "back-menu", "back-button"}))->setTarget(this, menu_selector(ModsOverlay::onGoogooGaga));
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
/*
Container* ModsOverlay::createInstalledTab() {
  auto ret = ModListContainer::create(m_provider, InstalledModList::get(godeLayer));
  ret->setName("Installed");
  return ret;
}

Container* ModsOverlay::createPopularTab() {
  auto ret = Container::create();
  ret->setName("Popular");
  return ret;
}
  */

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
