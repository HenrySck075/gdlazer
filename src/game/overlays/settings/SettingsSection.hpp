#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../../frameworks/graphics/sprites/IconConstructor.hpp"
#include "../../graphics/ui/OsuText.hpp"

GDL_NS_START

class SettingsSection : public frameworks::Container {
public:
  bool init(std::string header, frameworks::IconConstructor icon);
  static SettingsSection* create(std::string header, frameworks::IconConstructor icon) {
    $createClass(SettingsSection, init, header, icon);
  }

  void focus();
  void unfocus();

  void onEnter() override;

  std::string const& getHeader() {return m_header;};
  frameworks::CCResizableSprite* getIcon() {return m_icon;};

protected:
  void addSettings(frameworks::Container* settingsView);
  geode::Ref<OsuText> m_headerText;
  geode::Ref<CCLayerColor> m_unfocusedOverlay;
  
private:
  geode::Ref<frameworks::FillFlowContainer> m_content;
  std::string m_header;
  geode::Ref<frameworks::CCResizableSprite> m_icon;
  bool m_layoutLockedIn = false;

  void balls(float);
};

class SettingsSubsection : public SettingsSection {
public:
  bool init(std::string header);
  static SettingsSubsection* create(std::string header) {
    $createClass(SettingsSubsection, init, header);
  }
};

GDL_NS_END