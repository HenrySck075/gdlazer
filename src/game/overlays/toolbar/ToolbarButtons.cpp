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
  return ToolbarToggleButton::init(OsuIcon::Settings, "settings", "the", AxisAlignment::Start, "SettingsPanel");
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

bool tempForceReplace = false;
#include <Geode/modify/CCDirector.hpp>
struct hook51 : Modify<hook51, CCDirector>{
  bool replaceScene(CCScene* scene) {
    log::debug("geegjgofwejgviewjgvoiewnvfew");
    if (tempForceReplace && !dynamic_cast<CCTransitionScene*>(getRunningScene())) {
      tempForceReplace = false;
      return CCDirector::replaceScene(CCTransitionFade::create(0.5,OsuGame::get()));
    }
    else return CCDirector::replaceScene(scene);
  }
};
void ToolbarGeodeButton::ModsLayer_onBack(CCObject *) {
  (m_modsLayer->*ModsLayer_onBack_original)(nullptr);
};
void ToolbarGeodeButton::onClick(MouseEvent *e) {
  ToolbarButton::onClick(e);
  static_cast<CCMenuItemSpriteExtra *>(
    GameManager::sharedState()->m_menuLayer->getChildByIDRecursive(
      "geode.loader/geode-button"
    )
  )->activate();
  tempForceReplace = true;
}

