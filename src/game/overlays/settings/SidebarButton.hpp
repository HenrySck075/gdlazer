#pragma once

#include "../../graphics/ui/OsuButton.hpp"
#include "../OverlayColorProvider.hpp"
#include "../../../frameworks/bindables/Bindable.hpp"

GDL_NS_START

class SidebarButton : public OsuButton {
private:
  frameworks::Bindable<SidebarButton*>* m_currentItemPtr; friend class SettingsSidebar;
  struct ConstructorKwargs {
    OverlayColorProvider* colorProvider;
  };
public:
  static SidebarButton* create(GDF_KWARGS) {
    $createClass2(SidebarButton);
  }
  bool init(GDF_KWARGS);
};

GDL_NS_END