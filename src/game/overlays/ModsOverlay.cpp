#include "ModsOverlay.hpp"
#include "geodemods/ModItem.hpp"
#include "../../frameworks/graphics/containers/GridContainer.hpp"
//#include "../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../frameworks/graphics/containers/ScrollContainer.hpp"

GDL_NS_START
using namespace frameworks;

bool ModsOverlay::init() {
  auto body = Container::create();

  if (!WaveContainer::init(
    OverlayColorScheme::Purple,
    body
  )) return false;
  setName("Geode");

  body->setAnchor(Anchor::Bottom);

  /// Mods list (downloaded)
  auto installedMods = Loader::get()->getAllMods();
  auto iml = installedMods.size();
  int rows = ceil(iml/2.f);
  auto grid = GridContainer::create(
    // 2 autosized columns
    {
      Dimension{/*.mode = Dimension::Mode::Distributed*/},
      Dimension{/*.mode = Dimension::Mode::Distributed*/},
    },
    std::vector<Dimension>(rows, Dimension{})
  );
  grid->setGap(2);
  grid->reserve(rows, 2);
  log::debug("[ModsOverlay]: {} {} {}", iml, rows, iml/2.f);
  for (int i = 0; i < iml; i++) {
    int r = floor(i/2.f), c = i%2;
    auto mod = installedMods[i];
    grid->addChildAtCell(r, c, $verifyPtr(GeodeModItem::create(mod, m_provider)));
  }

  auto sc = ScrollContainer::create(grid);
  body->addChild(sc);
  sc->setMaxSize({300, 0});
  sc->setContentSize({100,100},Unit::Percent);
  grid->setAnchor(Anchor::Bottom);
  grid->setContentWidth(100,Unit::Percent);
  grid->setAnchorPoint({0.5,0});
  /// delay by 2 frames to ensure grid has a width
  queueInMainThread([grid, sc]{
    grid->updateLayout();
  });

  return true;
}

GDL_NS_END
