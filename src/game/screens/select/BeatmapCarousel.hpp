#pragma once
#include <Geode/Geode.hpp>
class Container;
#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../utils.hpp"
using namespace geode::prelude;

class BeatmapCarousel : public Container {
public:
    static BeatmapCarousel* create(GJGameLevel* level) {
        create_class(BeatmapCarousel, init, level);
    };
    bool init(GJGameLevel* level) {

    }
};