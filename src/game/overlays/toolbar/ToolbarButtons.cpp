#include "ToolbarButtons.hpp"
#include "../../OsuGame.hpp"
#include "Geode/cocos/cocoa/CCObject.h"

/**
 * ToolbarSettingsButton
 */

void ToolbarSettingsButton::select() {
    ToolbarToggleButton::select();
    OsuGame::get()->showSettings();
}

void ToolbarSettingsButton::deselect() {
    ToolbarToggleButton::deselect();
    OsuGame::get()->hideSettings();
}


/**
 * ToolbarMusicButton
 */

void ToolbarMusicButton::select() {
    ToolbarToggleButton::select();
    OsuGame::get()->pushOverlay(o);
}

void ToolbarMusicButton::deselect() {
    ToolbarToggleButton::deselect();
    OsuGame::get()->popOverlay(o);
}


/**
 * ToolbarGeodeButton
 */

// TODO: There are kore than this
void ToolbarGeodeButton::ModsLayer_onBack(CCObject *) {
    CCDirector::get()->popScene();
};
void ToolbarGeodeButton::onClick(MouseEvent *e) {
    ToolbarButton::onClick(e);
    static_cast<CCMenuItemSpriteExtra *>(
        GameManager::sharedState()->m_menuLayer->getChildByIDRecursive(
            "geode.loader/geode-button"
        )
    )->activate();

    queueInMainThread([]{
        auto s = CCDirector::get()->getRunningScene();
        if (auto transition = dynamic_cast<CCTransitionScene*>(s)) {
            static_cast<CCMenuItemSpriteExtra*>(
                reinterpret_cast<CCScene*>(
                    reinterpret_cast<uintptr_t>(transition)+0x148
                )->getChildByIDRecursive("back-button")
            )->m_pfnSelector = menu_selector(
                ToolbarGeodeButton::ModsLayer_onBack
            );
        }
    });
}

