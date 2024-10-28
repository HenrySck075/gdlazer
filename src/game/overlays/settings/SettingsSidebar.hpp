#pragma once 
#include "../../../framework/graphics/containers/Container.hpp"
#include "../../../utils.hpp"

class SettingsSidebar : public Container {
public: 
  static const float CONTRACTED_WIDTH;
  static const float EXPANDED_WIDTH;

  bool init();
  default_create(SettingsSidebar);
};