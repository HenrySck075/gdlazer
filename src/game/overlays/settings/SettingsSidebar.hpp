#pragma once 
#include "../../../frameworks/graphics/containers/Container.hpp"
#include "../../../utils.hpp"

GDL_NS_START

class SettingsSidebar : public frameworks::Container {
public: 
  static const float s_contractedWidth;
  static const float c_expandedWidth;

  bool init();
  $defaultCreate(SettingsSidebar);
};

GDL_NS_END
