#pragma once

#include "../../../framework/screens/Screen.hpp"
#include "DrawableCarouselBeatmap.hpp"

class SongSelect : public Screen {
public:
    bool init() {
        Screen::init();
        addChild(DrawableCarouselBeatmap::create(GameLevelManager::sharedState()->getMainLevel(1,true)));
    }
};