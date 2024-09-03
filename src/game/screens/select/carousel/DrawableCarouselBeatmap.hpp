#pragma once
#include <Geode/Geode.hpp>
class Container;
#include "../../../graphics/containers/OsuClickableContainer.hpp"
#include "../../../graphics/CCResizableSprite.hpp"
#include "../../../../utils.hpp"
using namespace geode::prelude;

namespace {
extern char const carouselItemSelect[] = "carouselItemSelect";
}

using CarouselItemSelect = NamedNodeEvent<carouselItemSelect>;

/// @note This is CarouselHeader
class DrawableCarouselBeatmap : public OsuClickableContainer {
private:
    GJGameLevel* m_level;
    CCClippingNode* m_main;
    CCScale9Sprite* m_shadow;
    CCResizableSprite* m_thumbnail;
public:
    float const MAX_HEIGHT = 80;
    float const h = MAX_HEIGHT*0.6;
    float const HEIGHT = h; // does not include padding

    static DrawableCarouselBeatmap* create(GJGameLevel* level) {
        create_class(DrawableCarouselBeatmap, init, level);
    };
    bool init(GJGameLevel* level);

    void onClick();
    void onMouseEnter();
    void onMouseExit();
    void onMouseUp() {};
    void onMouseDown() {};

    void select();
    void deselect();
};
