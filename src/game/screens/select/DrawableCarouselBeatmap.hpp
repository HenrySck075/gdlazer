#pragma once
#include <Geode/Geode.hpp>
class Container;
#include "../../graphics/containers/OsuClickableContainer.hpp"
#include "../../../utils.hpp"
using namespace geode::prelude;

/// @note plus im still confused
class DrawableCarouselBeatmap : public OsuClickableContainer {
private:
    GJGameLevel* m_level;
public:
    float const MAX_HEIGHT = 80;
    float const h = MAX_HEIGHT*0.6;
    float const HEIGHT = h; // does not include padding

    static DrawableCarouselBeatmap* create(GJGameLevel* level) {
        create_class(DrawableCarouselBeatmap, init, level);
    };
    bool init(GJGameLevel* level) {
        m_level = level;
        setContentSizeWithUnit(CCSize(100,h), Unit::OpenGL, Unit::UIKit);
        setOpacity(255);
        setColor(ccc3(255,0,0));
        return true;
    };

    void onClick() {};
    void onMouseEnter() {};
    void onMouseExit() {};
    void onMouseUp() {};
    void onMouseDown() {};
};