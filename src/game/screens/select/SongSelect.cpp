#include "SongSelect.hpp"

bool SongSelect::init() {
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
    carousel->setAnchorPoint({1,0.5});

    auto grid = GridContainer::create({
        Dimension(),
        // unfortunately you have to do this
        Dimension{GridSizeMode::Relative, 0, 0.5f, processUnit(850, Unit::UIKit, false)}
    });
    auto details = CCNode::create();// nothing burger for now
    grid->addChild(details); 
    grid->insertAfter(carousel, details);
    grid->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
    addChild(grid);

    return true;
}