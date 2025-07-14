#pragma once 
#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "SidebarButton.hpp"

GDL_NS_START

class SettingsSidebar : public frameworks::Container {
  geode::cocos::CCArrayExt<SidebarButton> m_sidebarItems;
  frameworks::Bindable<SidebarButton*> m_selectedButton;
  geode::Ref<frameworks::FillFlowContainer> m_content;
public: 
  static constexpr float s_contractedWidth = 70;
  static constexpr float c_expandedWidth = 170;

  bool init();
  $defaultCreate(SettingsSidebar);

  void addSidebarItem(SidebarButton* item);
  void updateLayout();
};

GDL_NS_END
