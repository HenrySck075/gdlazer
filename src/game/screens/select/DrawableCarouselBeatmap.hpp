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
        OsuClickableContainer::init("select-expand.wav"_spr, [](CCNode*self){});
        m_level = level;
        addChild(colorBg);
        setAnchorPoint(ccp(0,0));
        setContentSizeWithUnit(CCSize(0.4,h), Unit::Viewport, Unit::UIKit);
        setOpacity(255);
        setColor(ccc3(255,0,0));
        return true;

        //CCScheduler::get()->scheduleUpdateForTarget();
    };

    void onClick() {};
    void onMouseEnter() { };
    void onMouseExit() {};
    void onMouseUp() {};
    void onMouseDown() {};
};