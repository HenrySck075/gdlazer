#include "ToolbarButtons.hpp"
#include "../../OsuGame.hpp"
#include "Geode/cocos/cocoa/CCObject.h"
#include "../../../frameworks/input/events/KeyEvent.hpp"

GDL_NS_START

using namespace frameworks;

/**
 * ToolbarSettingsButton
 */

void ToolbarSettingsButton::select() {
  //OsuGame::get()->showSettings();
}

void ToolbarSettingsButton::deselect() {
  //OsuGame::get()->hideSettings();
}

bool ToolbarSettingsButton::init() {
  setID("settings");
  addListener<KeyEvent>([this](KeyEvent *e) {
    if (e->m_modifiers.ctrl && e->m_key == enumKeyCodes::KEY_O)
      select();

    return true;
  });
  return ToolbarToggleButton::init(OsuIcon::Settings, "settings", "the", AxisAlignment::Start, "SettingsPanel");
}

/**
 * ToolbarModDisableButton
 */
/*
bool ToolbarModDisableButton::init() { 
  // for why im not hooking/patching ModsLayer to override its onBack to pop the scene,
  // idk
  addListener<MouseEvent>([this](MouseEvent* e){
    if (e->m_eventType == MouseEventType::Click)  {
      Mod::get()->disable();
      geode::utils::game::restart();
    }
    return true;
  });
  return ToolbarButton::init(
    OsuIcon::CrossCircle,
    "(Prerelease feature) Disable mod",
    "disable this mf",
    AxisAlignment::End
  );
}
*/

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

GDL_NS_END

bool tempForceReplace = false;
#include <Geode/modify/CCDirector.hpp>
struct hook51 : Modify<hook51, CCDirector>{
  bool replaceScene(CCScene* scene) {
    if (tempForceReplace/* && !geode::cast::typeinfo_cast<CCTransitionScene*>(getRunningScene())*/) {
      tempForceReplace = false;
      return CCDirector::replaceScene(CCTransitionFade::create(0.5,gdlazer::game::OsuGame::get(false)));
    }
    else return CCDirector::replaceScene(scene);
  }
};

intptr_t ModsLayer_onBack_ptr = getGeodeLib();
void ModsLayer_onBack(void* a, CCObject* b) {
  tempForceReplace = true;
  ((void(*)(void*, CCObject*))(ModsLayer_onBack_ptr))(a,b);
}

GDL_NS_START
bool ToolbarGeodeButton::init() {
  bool h = ToolbarButton::init(
    OsuIcon::Gear,
    "geode mod loader",
    "manages the mods (F12)",
    AxisAlignment::End
  );
  addListener<MouseEvent>([this](MouseEvent* e) {
    if (e->m_eventType != MouseEventType::Click) return true;
    static_cast<CCMenuItemSpriteExtra *>(
      GameManager::sharedState()->m_menuLayer->getChildByIDRecursive(
        "geode.loader/geode-button"
      )
    )->activate();
    return true;
  });
  return h;
}

GDL_NS_END
