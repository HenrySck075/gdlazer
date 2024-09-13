#include "SettingsPanel.hpp"
#include "../OsuGame.hpp"
#include "../overlays/toolbar/ToolbarToggleButton.hpp"

const float SettingsPanel::sidebar_width = SettingsSidebar::EXPANDED_WIDTH;
const float SettingsPanel::TRANSITION_LENGTH = 0.6;
const float SettingsPanel::PANEL_WIDTH = 400;
const float SettingsPanel::WIDTH = SettingsPanel::sidebar_width + SettingsPanel::PANEL_WIDTH;

bool SettingsPanel::init() {
    if (!OverlayContainer::init()) return false;
    setColor({0,0,0});
    return true;
}

void SettingsPanel::onOpen() {
    runAction(CCFadeTo::create(0.2,180));
    OsuGame::get()->getChildByID("screens")->runAction(CCEaseOutQuint::create(CCMoveTo::create(SettingsPanel::TRANSITION_LENGTH, {(float)SettingsSidebar::EXPANDED_WIDTH/4,0})));
}
void SettingsPanel::onClose() {
    stopAllActions();
    runAction(CCFadeTo::create(0.2,0));
    OsuGame::get()->getChildByID("screens")->runAction(CCEaseOutQuint::create(CCMoveTo::create(SettingsPanel::TRANSITION_LENGTH, {0,0})));
}
// wacky ik
void SettingsPanel::onDismiss() {
    static_cast<ToolbarToggleButton*>(OsuGame::get()->getChildByIDRecursive("settings"))->deselect();
}