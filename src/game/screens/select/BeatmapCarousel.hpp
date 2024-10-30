#pragma once

#include <Geode/Geode.hpp>
#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../utils.hpp"
#include "carousel/DrawableCarouselBeatmap.hpp"
using namespace geode::prelude;

class BeatmapCarousel : public Container {
private:
  DrawableCarouselBeatmap* currentItem = nullptr;
public:
  bool init(GJLevelList* list);
  static BeatmapCarousel* create(GJLevelList* list) {
    $create_class(BeatmapCarousel, init, list);
  };
};
