#pragma once

#include <Geode/Geode.hpp>
#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../utils.hpp"
using namespace geode::prelude;

class BeatmapCarousel : public Container {
public:
    bool init(GJLevelList* list);
    static BeatmapCarousel* create(GJLevelList* list) {
        create_class(BeatmapCarousel, init, list);
    };
};