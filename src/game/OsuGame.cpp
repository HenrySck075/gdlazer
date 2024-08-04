#include "OsuGame.hpp"

bool OsuGame::init() {
    Toolbar* toolbar = Toolbar::create();
    this->addChild(toolbar);
    toolbar->toolbarStyle->offset->setObject(ccp(0,toolbar->HEIGHT));
    toolbar->toolbarStyle->offset_unit->vertical->setObject(Unit::UIKit);

    this->addChild(MainMenu::create(false));
    return true;
}