#include "ModsOverlay.hpp"

GDL_NS_START
using namespace frameworks;

bool ModsOverlay::init() {
  auto body = Container::create();

  return WaveContainer::init(
    OverlayColorScheme::Purple,
    body
  );
}

GDL_NS_END