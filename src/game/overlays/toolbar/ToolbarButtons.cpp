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


bool ToolbarNativeSettingsButton::init() {
  setID("native-settings");
  addListener<MouseEvent>([this](MouseEvent* e) {
    if (e->m_eventType == MouseEventType::Click && isMouseEntered()) {
      // Open native settings
      MenuLayer::get()->onOptions(nullptr);
    }
    return true;
  });
  return ToolbarButton::init(OsuIcon::Settings, "settings", "the", AxisAlignment::End);
}

/**
 * ToolbarGeodeButton
 */
bool ToolbarGeodeButton::init() {
  return ToolbarToggleButton::init(
    OsuIcon::Maintenance,
    "mods list",
    "manages geode mods (F12)",
    AxisAlignment::End,
    "Geode"
  );
}

void ToolbarGeodeButton::select() {
  (m_modsLayer = ModsOverlay::create())->show();
}
void ToolbarGeodeButton::deselect() {
  if (m_modsLayer) m_modsLayer->hide();
}

GDL_NS_END
