#pragma once

#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../../frameworks/graphics/sprites/IconConstructor.hpp"

GDL_NS_START

class SettingsSection : public frameworks::Container {
public:
  bool init(std::string header, frameworks::IconConstructor icon);
  static SettingsSection* create(std::string header, frameworks::IconConstructor icon) {
    $createClass(SettingsSection, init, header, icon);
  }

  void focus();
  void unfocus();

  std::string const& getHeader() {return m_header;};
  frameworks::CCResizableSprite* getIcon() {return m_icon;};

protected:
  void addSettingsContainer(frameworks::Container* settingsView);
  
private:
  frameworks::FillFlowContainer* m_content;
  std::string m_header;
  frameworks::CCResizableSprite* m_icon;
  CCLayerColor* m_unfocusedOverlay;
};

GDL_NS_END