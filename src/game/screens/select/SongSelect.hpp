#pragma once

#include "../../../framework/screens/Screen.hpp"
#include "../../../framework/graphics/containers/GridContainer.hpp"
#include "../../../utils.hpp"
#include "../../../framework/graphics/sprites/CCResizableSprite.hpp"
#include "carousel/DrawableCarouselBeatmap.hpp"
#include "BeatmapCarousel.hpp"

class SongSelect : public Screen {
public:
  static SongSelect* create() {
    create_class(SongSelect,init);
  }
  bool init();
};