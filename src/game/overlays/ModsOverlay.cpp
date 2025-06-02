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

  body->setMaxSize({250, 0});
  body->setAnchor(Anchor::Bottom);

  /// Mods list (downloaded)
  auto installedMods = Loader::get()->getAllMods();
  auto iml = installedMods.size();
  
  auto grid = GridContainer::create(
    // 2 autosized columns
    {
      Dimension{.mode = Dimension::Mode::Distributed},
      Dimension{.mode = Dimension::Mode::Distributed},
    },
    std::vector<Dimension>(ceil(iml/2), {})
  );
  grid->setGap(8);
  grid->reserve(ceil(iml/2), 2);
  for (int i = 0; i < iml; i++) {
    int r = i/2, c = i%2;
    auto mod = installedMods[i];
    grid->addChildAtCell(r, c, $verifyPtr(GeodeModItem::create(mod, m_provider)));
  }

  auto sc = ScrollContainer::create(grid);
  body->addChild(sc);
  sc->setContentSize({100,100},Unit::Percent);
  grid->setContentSize({100,0},Unit::Percent);
  grid->setAnchor(Anchor::Bottom);
  grid->setAnchorPoint({0.5,0});
  /// delay by 2 frames to ensure grid has a width
  queueInMainThread([grid, sc]{
    grid->updateLayout();
    //sc->resizeToChildSize();
  });

  return true;
}

GDL_NS_END