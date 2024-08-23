#pragma once
#include <Geode/Geode.hpp>
class Container;
#include "../../../graphics/containers/OsuClickableContainer.hpp"
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
public:
    float const MAX_HEIGHT = 80;
    float const h = MAX_HEIGHT*0.6;
    float const HEIGHT = h; // does not include padding

    static DrawableCarouselBeatmap* create(GJGameLevel* level) {
        create_class(DrawableCarouselBeatmap, init, level);
    };
    bool init(GJGameLevel* level);

    void onClick() {
        m_shadow->setColor(ccc3(130, 204, 255));
        m_shadow->setOpacity(150);
        static_cast<Container*>(getParent())->dispatchEvent(new CarouselItemSelect());
    };
    void onMouseEnter() {
        colorBg->stopAllActions();
        colorBg->runAction(CCFadeTo::create(0.1,25));
    };
    void onMouseExit() {
        colorBg->stopAllActions();
        colorBg->runAction(CCFadeTo::create(0.25,0));
    };
    void onMouseUp() {};
    void onMouseDown() {};
};