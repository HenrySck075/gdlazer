#include "SettingsSidebar.hpp"
#include "../OverlayColorProvider.hpp"

const float SettingsSidebar::CONTRACTED_WIDTH = 70;
const float SettingsSidebar::EXPANDED_WIDTH = 170;

bool SettingsSidebar::init() {
  if (!Container::init()) return false;
  setContentSizeWithUnit({EXPANDED_WIDTH,100},Unit::UIKit,Unit::Percent);
  setColor(OverlayColorProvider::create(OverlayColorScheme::Purple)->Background5());
  setOpacity(255);
  return true;
}