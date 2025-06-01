#include "ModsOverlay.hpp"
#include "geodemods/ModItem.hpp"
//#include "../../frameworks/graphics/containers/GridContainer.hpp"
#include "../../frameworks/graphics/containers/FillFlowContainer.hpp"

GDL_NS_START
using namespace frameworks;

bool ModsOverlay::init() {
  auto body = Container::create();

  if (!WaveContainer::init(
    OverlayColorScheme::Purple,
    body
  )) return false;

  body->setMaxSize({300, -1});

  /// Mods list (downloaded)
  auto installedMods = Loader::get()->getAllMods();
  /// i dont trust my system at all (probably need a rewrite)
  
  auto grid = FillFlowContainer::create(FillDirection::Vertical);
  grid->setGap(8);
  for (auto mod : installedMods) {
    grid->addChild($verifyPtr(GeodeModItem::create(mod, m_provider)));
  }

  body->addChild(grid);
  grid->setContentSize({100,100},Unit::Percent);
  queueInMainThread([grid]{
    grid->updateLayout();
  });

  return true;
}

GDL_NS_END