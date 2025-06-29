#pragma once

#include "../graphics/containers/OsuOverlayContainer.hpp"
#include "../graphics/containers/SectionsContainer.hpp"
#include "../../utils.hpp"
#include "settings/SettingsSidebar.hpp"

GDL_NS_START

class SettingsPanel : public OsuOverlayContainer {
protected:
  SettingsSidebar* m_sidebar;
  Container* m_mainPanel;
public:
  static constexpr float c_contentMargins = 20;

  static constexpr float c_transitionLength = 0.6;

  private: static constexpr float c_sidebarWidth = 400;

public: 
  /// <summary>
  /// The width of the settings panel content, excluding the sidebar.
  /// </summary>
  static constexpr float c_panelWidth = SettingsSidebar::c_expandedWidth;

  /// <summary>
  /// The full width of the settings panel, including the sidebar.
  /// </summary>
  static constexpr float c_width = SettingsPanel::c_sidebarWidth + SettingsPanel::c_panelWidth;

  bool init();
  static SettingsPanel* create() {
    $createClass(SettingsPanel, init);
  }
  virtual CCArrayExt<Container*> createSections() {return CCArray::create();};
  void onOpen() override;
  void onClose() override;
};

// SectionsContainer with the fade stuff idk
class SettingsSections : public frameworks::SectionsContainer {
public:
  void onSectionSelect(Container* new_) override;
  $defaultCreate(SettingsSections);
};

GDL_NS_END
