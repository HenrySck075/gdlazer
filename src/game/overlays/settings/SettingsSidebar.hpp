#pragma once 
#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../utils.hpp"

GDL_NS_START

class SettingsSidebar : public frameworks::Container {
public: 
  static constexpr float s_contractedWidth = 70 * 2;
  static constexpr float c_expandedWidth = 170 * 2;

  bool init();
  $defaultCreate(SettingsSidebar);
};

GDL_NS_END
