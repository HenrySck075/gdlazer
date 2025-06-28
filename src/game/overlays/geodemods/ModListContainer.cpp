#include "ModListContainer.hpp"
#include "../../../frameworks/graphics/containers/GridContainer.hpp"
#include "../../../frameworks/graphics/containers/ScrollContainer.hpp"


GDL_NS_START
using namespace frameworks;

bool ModListContainer::init(OverlayColorProvider* provider, ModList* list) {
  if (!Container::init()) return false;
  /*
  auto grid = GridContainer::create();
  auto sc = ScrollContainer::create(grid, ScrollDirection::Vertical, geode::AxisAlignment::Center);
  grid->setGap(8);
  
  CCArrayExt<Container> e;
  for (auto mod : list->loadNextModsList()) {
    e.push_back($verifyPtr(GeodeModItem::create(mod, provider)));
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

  addChild(sc);
  */
  return true;
}

GDL_NS_END