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

bool tempSwapReplace = false;
#include <Geode/modify/CCDirector.hpp>
struct hook51 : Modify<hook51, CCDirector>{
    void replaceScene(CCScene* scene) {
        if (tempSwapReplace) popScene();
        else replaceScene(scene);
    }
};
void ToolbarGeodeButton::ModsLayer_onBack(CCObject *) {
    tempSwapReplace = true;
    (m_modsLayer->*ModsLayer_onBack_original)(nullptr);
    tempSwapReplace = false;
};
void ToolbarGeodeButton::onClick(MouseEvent *e) {
    ToolbarButton::onClick(e);
    static_cast<CCMenuItemSpriteExtra *>(
        GameManager::sharedState()->m_menuLayer->getChildByIDRecursive(
            "geode.loader/geode-button"
        )
    )->activate();

    queueInMainThread([this]{
        auto s = CCDirector::get()->getRunningScene();
        if (auto transition = dynamic_cast<CCTransitionScene*>(s)) {
            auto b = static_cast<CCScene*>(
                // see main.cpp:127
                transition->getUserObject("m_pInScene")
            );
            m_modsLayer = static_cast<CCLayer*>(b->getChildByID("ModsLayer"));
            auto node = static_cast<CCMenuItemSpriteExtra*>(
                b->getChildByIDRecursive("back-button")
            );
            ModsLayer_onBack_original = node->m_pfnSelector;
            node->m_pfnSelector = menu_selector(
                ToolbarGeodeButton::ModsLayer_onBack
            );
            
        }
    });
}

