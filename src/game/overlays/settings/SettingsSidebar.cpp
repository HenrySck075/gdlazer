#include "SettingsSidebar.hpp"
#include "../OverlayColorProvider.hpp"

GDL_NS_START
using namespace frameworks;

const float SettingsSidebar::s_contractedWidth = 70;
const float SettingsSidebar::c_expandedWidth = 170;

bool SettingsSidebar::init() {
  if (!Container::init()) return false;
  setContentSize({c_expandedWidth,100},Unit::UIKit,Unit::Percent);
  setBackgroundColor(OverlayColorProvider::create(OverlayColorScheme::Purple)->Background5());
  setOpacity(255);
  return true;
}

GDL_NS_END
