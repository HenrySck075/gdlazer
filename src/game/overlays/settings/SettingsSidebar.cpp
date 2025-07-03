#include "SettingsSidebar.hpp"
#include "../OverlayColorProvider.hpp"

GDL_NS_START
using namespace frameworks;

bool SettingsSidebar::init() {
  if (!Container::init()) return false;
  setContentSize({c_expandedWidth,100},Unit::UIKit,Unit::Percent);
  setBackgroundColor(OverlayColorProvider::create(OverlayColorScheme::Purple)->Background5());
  setOpacity(255);
  return true;
}

void SettingsSidebar::addSidebarItem(SidebarButton* item) {
  item->m_currentItemPtr = &m_selectedButton;
  m_sidebarItems.push_back(item);
  addChild(item);
}

GDL_NS_END
