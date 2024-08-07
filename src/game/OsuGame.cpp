#include "OsuGame.hpp"
#include "../helpers/CustomActions.hpp"

bool OsuGame::init() {
    CCScene::init();
    main = Container::create();
    main->setContentSize(getContentSize());
    this->addChild(main);
    main->addChild(MainMenu::create(false));
    toolbar = Toolbar::create();
    this->addChild(toolbar);

    return true;
}

void OsuGame::showToolbar() {
    toolbar->show();
    main->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,main->getContentWidth(),main->getContentHeight()-toolbar->HEIGHT)
    ));
}

void OsuGame::hideToolbar() {
    toolbar->hide();
    main->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,main->getContentWidth(),getContentHeight())
    ));
}