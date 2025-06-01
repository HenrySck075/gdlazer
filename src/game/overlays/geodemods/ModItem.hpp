/// Based on BeatmapCard

#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../overlays/OverlayColorProvider.hpp"

GDL_NS_START

class GeodeModItem : public frameworks::Container {
public:
  static GeodeModItem* create(geode::Mod* mod, OverlayColorProvider* provider);

  bool init(geode::Mod* mod, OverlayColorProvider* provider);

  constexpr static const float WIDTH = 300;
  constexpr static const float HEIGHT = 60;
private:
  geode::Mod* m_mod;

  constexpr static float TRANSITION_DURATION = 0.34f;
  constexpr static float CORNER_RADIUS = 8;
};

GDL_NS_END