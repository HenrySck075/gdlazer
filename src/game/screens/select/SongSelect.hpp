#pragma once

#include "../../../framework/screens/Screen.hpp"
#include "../../../utils.hpp"
#include "../../graphics/CCResizableSprite.hpp"
#include "carousel/DrawableCarouselBeatmap.hpp"
#include "BeatmapCarousel.hpp"

class SongSelect : public Screen {
public:
    static SongSelect* create() {
        create_class(SongSelect,init);
    }
    bool init() {
        Screen::init();
        m_title = "Level select";
        auto testBg = CCResizableSprite::createWithSpriteFrameName("menu-background-5.png"_spr);
        testBg->setContentSize(CCDirector::sharedDirector()->getWinSize());
        testBg->setPosition(CCDirector::sharedDirector()->getWinSize()/2);
        addChild(testBg);
        auto glm = GameLevelManager::sharedState();
        auto mainList = GJLevelList::create();
        for (int i = 1; i <= 22; i++) {
            mainList->addLevelToList(glm->getMainLevel(i, true));
        }
        //addChild(DrawableCarouselBeatmap::create(glm->getMainLevel(1,true)));
        auto carousel = BeatmapCarousel::create(mainList);
        carousel->setAnchor(Anchor::Right);
        carousel->setAnchorPoint(ccp(1,0.5));
        addChild(carousel);

        return true;
    }
};