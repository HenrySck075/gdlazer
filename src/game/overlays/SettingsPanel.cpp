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
    sidebar = SettingsSidebar::create();
    sidebar->setPositionWithUnit({-SettingsSidebar::EXPANDED_WIDTH,0},Unit::UIKit,Unit::OpenGL);
    addChild(sidebar);
    return true;
}

void SettingsPanel::onOpen() {
    stopAllActions();
    sidebar->stopAllActions();
    runAction(CCSequence::createWithTwoActions(
        CCFadeTo::create(0.2,180),
        CCDelayTime::create(SettingsPanel::TRANSITION_LENGTH-0.2)
    ));
    sidebar->runAction(CCEaseOutQuint::create(
        CCMoveTo::create(SettingsPanel::TRANSITION_LENGTH,{0,0})
    ));
    auto s = OsuGame::get()->getChildByID("screens");
    s->stopActionByTag(7);
    s->runAction(CCEaseOutQuint::create(
        CCMoveTo::create(
            SettingsPanel::TRANSITION_LENGTH, 
            {(float)SettingsSidebar::EXPANDED_WIDTH/4,0}
        )
    ))->setTag(7);
}
void SettingsPanel::onClose() {
    stopAllActions();
    sidebar->stopAllActions();
    runAction(CCSequence::createWithTwoActions(
        CCFadeTo::create(0.2,0),
        CCDelayTime::create(SettingsPanel::TRANSITION_LENGTH-0.2)
    ));
    sidebar->runAction(CCEaseOutQuint::create(
        CCMoveTo::create(
            SettingsPanel::TRANSITION_LENGTH,
            {-SettingsSidebar::EXPANDED_WIDTH,0}
        )
    ));

    auto s = OsuGame::get()->getChildByID("screens");
    s->stopActionByTag(7);
    s->runAction(CCEaseOutQuint::create(
        CCMoveTo::create(SettingsPanel::TRANSITION_LENGTH, {0,0})
    ))->setTag(7);
}
// wacky ik
void SettingsPanel::onDismiss() {
    // static_cast<ToolbarToggleButton*>(OsuGame::get()->getChildByIDRecursive("settings"))->deselect();
}


/**
 * SettingsSections
 */

void SettingsSections::onSectionSelect(Container* old, Container* new_) {
    SectionsContainer::onSectionSelect(old, new_);
    old->setOpacity(200);
    new_->setOpacity(255);
};
