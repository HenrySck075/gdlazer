#include "OsuGame.hpp"

bool OsuGame::init() {
    Toolbar* toolbar = Toolbar::create();
    this->addChild(toolbar);
    toolbar->setPositionY(toolbar->getPositionY()+toolbar->HEIGHT);

    //this->addChild(MainMenu::create(false));
    return true;
}