#pragma once

#include "ToolbarButton.hpp"
#include "ToolbarToggleButton.hpp"
#include "../../graphics/ui/OsuIcon.hpp"
#include "../ModsOverlay.hpp"
#include "../NowPlayingOverlay.hpp"
#include "../SettingsPanel.hpp"

GDL_NS_START

class ToolbarSettingsButton : public ToolbarToggleButton {
  geode::Ref<SettingsPanel> m_settingsOverlay;
public:
  bool init();
  $defaultCreate(ToolbarSettingsButton);
  void select() override;
  void deselect() override;
};

class ToolbarHomeButton : public ToolbarButton {
public:
  static ToolbarHomeButton* create() {
    $createClass(ToolbarHomeButton, init);
  }
  bool init() {
    setID("home");
    return ToolbarButton::init(OsuIcon::Home, "home", "return to the main menu");
  }
};


class ToolbarNativeSettingsButton : public ToolbarButton {
public:
  bool init();
  $defaultCreate(ToolbarNativeSettingsButton);
};

/// temporary impl
class ToolbarGeodeButton : public ToolbarToggleButton {
  geode::Ref<ModsOverlay> m_modsLayer;
public:
  $defaultCreate(ToolbarGeodeButton);
  bool init(); 

  void select() override;
  void deselect() override;
};
/*
class ToolbarModDisableButton : public ToolbarButton {
public:
  $defaultCreate(ToolbarModDisableButton);
  bool init();
};
*/

class ToolbarMusicButton : public ToolbarToggleButton {
  geode::Ref<NowPlayingOverlay> m_overlay;
public:
  $defaultCreate(ToolbarMusicButton);
  bool init();
  void select() override;
  void deselect() override;
};

/// TODO: convert to a toggle button when the dedicated overlay is made
class ToolbarUserButton : public ToolbarButton {
public:
  $defaultCreate(ToolbarUserButton);
  bool init();
  /*
  void select() override;
  void deselect() override;
  */
};
GDL_NS_END
