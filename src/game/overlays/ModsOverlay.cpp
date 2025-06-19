#include "ModsOverlay.hpp"
#include "geodemods/ModItem.hpp"
#include "../../frameworks/graphics/containers/GridContainer.hpp"
//#include "../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../frameworks/graphics/containers/ScrollContainer.hpp"
#include "../graphics/containers/TabContainer.hpp"

GDL_NS_START
using namespace frameworks;

bool ModsOverlay::init() {
  if (!WaveContainer::init(
    OverlayColorScheme::Purple
  )) return false;
  
  auto tabContainer = $verifyPtr(TabContainer::create({
    createInstalledTab()
  }, m_provider->Color0()));
  
  m_main->addChild(tabContainer);
  m_main->setPadding({4, 0});
  setName("Geode");
  setTitle("Geode Mods Listing");

  m_useContainerSizeForWaves = true;

  addChild(m_bg = SwelvyBG::create());
  m_bg->setZOrder(m_main->getZOrder()-1);
  
  addListener<NodeSizeUpdated>([this](NodeSizeUpdated*) {
    m_bg->setContentSize(getContentSize());
    return true;
  });
  return true;
}

Container* ModsOverlay::createInstalledTab() {
  auto grid = GridContainer::create();
  auto sc = ScrollContainer::create(grid, ScrollDirection::Vertical, geode::AxisAlignment::Center);
  sc->setName("Installed");
  
  /// Mods list (downloaded)
  auto installedMods = Loader::get()->getAllMods();
  auto iml = installedMods.size();
  int rows = ceil(iml/2.f);
  grid->setGap(8);
  
  CCArrayExt<Container> e;
  for (auto mod : installedMods) {
    e.push_back($verifyPtr(GeodeModItem::create(mod, m_provider)));
  }

  //sc->setMaxSize({300, 0});
  sc->setContentSize({100,100},Unit::Percent);
  sc->setAnchor(Anchor::Bottom);
  sc->setAnchorPoint({0.5,0});
  grid->setContentWidth(100,Unit::Percent);

  //body->setAnchor(Anchor::Bottom);
  /// delay by 2 frames to ensure grid has a size
  queueInMainThread([grid, sc, e]{
    grid->figureCellPlacementsFromChildrenList(e);
    grid->updateLayout();
    //sc->resizeToChildSize();
  });


  return sc;
}

void ModsOverlay::onOpen() {
  WaveContainer::onOpen();
  float h = getContentHeight();
  m_bg->setPositionY(m_main->getPositionY());
  m_bg->runAction(CCEaseSineOut::create(CCMoveTo::create(0.8f, {0,h})));
}

void ModsOverlay::onClose() {
  WaveContainer::onClose();
  float h = getContentHeight();
  m_bg->runAction(CCEaseSineIn::create(CCMoveTo::create(0.5f, {0,m_pos4-15})));
}
GDL_NS_END
