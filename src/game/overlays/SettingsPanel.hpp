#pragma once

#include "../graphics/containers/OsuOverlayContainer.hpp"
#include "../graphics/containers/SectionsContainer.hpp"
#include "settings/SettingsSection.hpp"
#include "settings/SettingsSidebar.hpp"

GDL_NS_START

// SectionsContainer with the fade stuff idk
class SettingsSections : public frameworks::SectionsContainer {
public:
  void onSectionSelect(Container* new_) override;
  $defaultCreate(SettingsSections);
};

/// According to the osu!lazer source this is the base structure for a settings panel.
/// Figured that it was for keybinding panel too should I do that
class SettingsPanel : public OsuOverlayContainer {
protected:
  geode::Ref<SettingsSidebar> m_sidebar;
  geode::Ref<frameworks::Container> m_mainPanel;

  geode::Ref<SettingsSections> m_sectionsContainer;

  geode::cocos::CCArrayExt<SettingsSection> m_sections;

private: static constexpr float c_sidebarWidth = SettingsSidebar::c_expandedWidth;
public: 
  static constexpr float c_contentMargins = 20;

  static constexpr float c_transitionLength = 0.6;

  /// The width of the settings panel content, excluding the sidebar.
  static constexpr float c_panelWidth = 400;

  /// The full width of the settings panel, including the sidebar.
  static constexpr float c_width = SettingsPanel::c_sidebarWidth + SettingsPanel::c_panelWidth;

  bool init();
  static SettingsPanel* create() {
    $createClass(SettingsPanel, init);
  }
  /// Returns a list of settings sections
  virtual CCArrayExt<SettingsSection> createSections();
  CCArrayExt<SidebarButton> createSidebarButtons();
  void onOpen() override;
  void onClose() override;
};

GDL_NS_END
