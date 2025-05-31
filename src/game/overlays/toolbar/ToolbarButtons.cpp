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

#include "../ModsOverlay.hpp"

GDL_NS_START
bool ToolbarGeodeButton::init() {
  bool h = ToolbarButton::init(
    OsuIcon::Maintenance,
    "mods list",
    "manages geode mods (F12)",
    AxisAlignment::End
  );
  addListener<MouseEvent>([this](MouseEvent* e) {
    if (e->m_eventType != MouseEventType::Click) return true;
    /*
    static_cast<CCMenuItemSpriteExtra *>(
      GameManager::sharedState()->m_menuLayer->getChildByIDRecursive(
        "geode.loader/geode-button"
      )
    )->activate();
    */
    ModsOverlay::create()->show();
    return true;
  });
  return h;
}

GDL_NS_END
