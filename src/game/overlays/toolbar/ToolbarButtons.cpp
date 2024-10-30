#include "ToolbarButtons.hpp"
#include "../../OsuGame.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "../../../framework/input/events/KeyEvent.hpp"
/**
 * ToolbarSettingsButton
 */

void ToolbarSettingsButton::select() {
  OsuGame::get()->showSettings();
}

void ToolbarSettingsButton::deselect() {
  OsuGame::get()->hideSettings();
}

bool ToolbarSettingsButton::init() {
  setID("settings");
  addListener("keyboardEvent", [this](NodeEvent *ev) {
    auto e = static_cast<KeyboardEvent *>(ev);
    if (e->key.ctrl && e->key.key == enumKeyCodes::KEY_O)
      select();
  });
  addListener("overlayEvent", [this](NodeEvent* e){
    log::debug("[ToolbarSettingsButton]: e");
    auto ev = static_cast<OverlayEvent*>(e);
    // todo: reliable way to determine if its SettingsPanel
    if (ev->getEventType() == OverlayEvent::Type::Popin) {
      ToolbarToggleButton::select();
    } else {
      ToolbarToggleButton::deselect();
    }
  });
  return ToolbarToggleButton::init(OsuIcon::Settings, "settings", "the");
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

