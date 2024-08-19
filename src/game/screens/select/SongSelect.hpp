#pragma once

#include "../../../framework/screens/Screen.hpp"
#include "../../../utils.hpp"
#include "../../graphics/CCResizableSprite.hpp"
#include "carousel/DrawableCarouselBeatmap.hpp"

class SongSelect : public Screen {
public:
    static SongSelect* create() {
        create_class(SongSelect,init);
    }
    bool init() {
        Screen::init();
        auto testBg = CCResizableSprite::createWithSpriteFrameName("menu-background-5.png"_spr);
        testBg->setContentSize(CCDirector::sharedDirector()->getWinSize());
        testBg->setPosition(CCDirector::sharedDirector()->getWinSize()/2);
        addChild(testBg);
        addChild(DrawableCarouselBeatmap::create(GameLevelManager::sharedState()->getMainLevel(1,true)));
        return true;
    }
};