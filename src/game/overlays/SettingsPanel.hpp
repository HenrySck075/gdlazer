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
  const float c_contentMargins = 20;

  static const float c_transitionLength;

  private: static const float c_sidebarWidth;

public: 
  /// <summary>
  /// The width of the settings panel content, excluding the sidebar.
  /// </summary>
  static const float c_panelWidth;

  /// <summary>
  /// The full width of the settings panel, including the sidebar.
  /// </summary>
  static const float c_width;

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
