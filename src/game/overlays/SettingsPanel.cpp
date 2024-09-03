#include "SettingsPanel.hpp"

bool SettingsPanel::init() {
    Container::init();
    return true;
}

void SettingsPanel::onOpen() {
    OsuGame::get()->getChildByID("screens")->runAction(CCMoveTo::create(0.5, CCPoint()));
}
