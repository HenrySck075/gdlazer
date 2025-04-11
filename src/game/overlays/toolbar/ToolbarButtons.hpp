#pragma once

#include "ToolbarButton.hpp"
#include "ToolbarToggleButton.hpp"
#include "../NowPlayingOverlay.hpp"
#include "../../graphics/OsuIcon.hpp"

GDL_NS_START

class ToolbarSettingsButton : public ToolbarToggleButton {
public:
  bool init();
  $default_create(ToolbarSettingsButton);
  void select() override;
  void deselect() override;
};

class ToolbarHomeButton : public ToolbarButton {
public:
  static ToolbarHomeButton* create() {
    $create_class(ToolbarHomeButton, init);
  }
  bool init() {
    setID("home");
    return ToolbarButton::init(OsuIcon::Home, "home", "return to the main menu");
  }
};


/// temporary impl
class ToolbarGeodeButton : public ToolbarButton {
  void ModsLayer_onBack(CCObject *);
  SEL_MenuHandler ModsLayer_onBack_original;
  CCLayer* m_modsLayer;
public:
  $default_create(ToolbarGeodeButton);
  bool init(); 
};

class ToolbarModDisableButton : public ToolbarButton {
public:
  $default_create(ToolbarModDisableButton);
  bool init();
};

class ToolbarMusicButton : public ToolbarToggleButton {
  NowPlayingOverlay* o;
public:
  $default_create(ToolbarMusicButton);
  bool init() {
    setID("music");
    o = NowPlayingOverlay::create();
    o->retain();
    return ToolbarToggleButton::init(
      OsuIcon::Music, 
      "now playing", 
      "manage the currently playing track (F6)", 
      AxisAlignment::End,
      "NowPlayingOverlay"
    );
  }
  void select() override;
  void deselect() override;

  ~ToolbarMusicButton() {
    o->release();
  }
};

GDL_NS_END
