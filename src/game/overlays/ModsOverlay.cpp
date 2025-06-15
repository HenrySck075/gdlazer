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
  
  auto grid = GridContainer::create();
  auto sc = ScrollContainer::create(grid);
  sc->setName("Installed");
  
  auto tabContainer = $verifyPtr(TabContainer::create({sc}, m_provider->Color0()));
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

  m_main->addChild(tabContainer);
  m_main->setPadding({4, 0});
  setName("Geode");
  setTitle("Geode Mods Listing");

  //body->setAnchor(Anchor::Bottom);
  /// delay by 2 frames to ensure grid has a size
  queueInMainThread([grid, sc, e]{
    grid->figureCellPlacementsFromChildrenList(e);
    grid->updateLayout();
    sc->resizeToChildSize();
  });
  return true;
}

GDL_NS_END
