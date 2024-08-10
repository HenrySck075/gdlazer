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

void OsuGame::pushScreen(Screen* s) {
    Screen* ls;
    if (screenStack.size()!=0) {
        ls = screenStack[screenStack.size()-1];
    }
    s->onEntering(ScreenTransitionEvent(ls,s));
    screenStack.push_back(s);
    main->addChild(s);
}

void OsuGame::popScreen() {
    if (screenStack.size()==0) {
        log::error("[OsuGame]: nice >:]");
        return;
    }
    auto s = screenStack[screenStack.size()-1];
    Screen* ps;
    if (screenStack.size()>1) {
        ps = screenStack[screenStack.size()-2];
    }
    
    ScreenTransitionEvent event = {s,ps};
    s->onExiting(event);
    if (ps!=nullptr) ps->onEntering(event);
    screenStack.pop_back();
}