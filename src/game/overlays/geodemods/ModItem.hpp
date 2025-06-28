/// Based on BeatmapCard

#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../overlays/OverlayColorProvider.hpp"

GDL_NS_START

struct ModEntry {
  std::string name;
  std::string developers;
  std::string version;

  CCMenuItemSpriteExtra* viewButton;
  geode::LazySprite* icon;

  bool featured;
  bool enabled;
  bool restartRequired;
  bool updateAvailable;
};

class GeodeModItem : public frameworks::Container {
public:
  static GeodeModItem* create(Mod* mod, OverlayColorProvider* provider);

  bool init(Mod* mod, OverlayColorProvider* provider);

  constexpr static const float WIDTH = 400;
  constexpr static const float HEIGHT = 80;
private:
  Mod* m_mod;

  constexpr static float TRANSITION_DURATION = 0.34f;
  constexpr static float CORNER_RADIUS = 8;
};

GDL_NS_END