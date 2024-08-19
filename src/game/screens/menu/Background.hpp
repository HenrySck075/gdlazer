//https://osu.ppy.sh/api/v2/seasonal-backgrounds
#pragma once

#include "../../../framework/graphics/containers/Container.hpp"
#include "../../graphics/containers/ParallaxContainer.hpp"
#include "../../../utils.hpp"
#include <henrysck075.imagecache/include/ImageCache.hpp>

class Background : public Container {
public:
    static Background* create() {
        create_class(Background, init);
    };
    bool init();
};