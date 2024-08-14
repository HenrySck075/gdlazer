#pragma once

#include "../../../framework/screens/Screen.hpp"
#include "../../../utils.hpp"
#include "DrawableCarouselBeatmap.hpp"

class SongSelect : public Screen {
public:
    static SongSelect* create() {
        create_class(SongSelect,init);
    }
    bool init() {
        Screen::init();
        addChild(DrawableCarouselBeatmap::create(GameLevelManager::sharedState()->getMainLevel(1,true)));
        return true;
    }
};