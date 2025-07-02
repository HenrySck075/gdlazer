#pragma once 
#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../utils.hpp"

GDL_NS_START

class SettingsSidebar : public frameworks::Container {
public: 
  static constexpr float s_contractedWidth = 70;
  static constexpr float c_expandedWidth = 170;

  bool init();
  $defaultCreate(SettingsSidebar);

  void addSidebarItem(SidebarButton* item);
};

GDL_NS_END
