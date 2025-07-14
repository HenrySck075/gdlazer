#include "SettingsSidebar.hpp"
#include "../OverlayColorProvider.hpp"

GDL_NS_START
using namespace frameworks;

bool SettingsSidebar::init() {
  if (!Container::init()) return false;
  setContentSize({c_expandedWidth,100},Unit::UIKit,Unit::Percent);
  setBackgroundColor(OverlayColorProvider::create(OverlayColorScheme::Purple)->Background5());
  setOpacity(255);

  addChild(m_content = FillFlowContainer::create(FillDirection::Vertical,Anchor::Top));
  m_content->setGap(2);
  m_content->setContentSize({100,100}, Unit::Percent);
  m_content->setAnchor(Anchor::TopLeft);
  m_content->setAnchorPoint({0,1});
  
  return true;
}

void SettingsSidebar::addSidebarItem(SidebarButton* item) {
  item->m_currentItemPtr = &m_selectedButton;
  m_sidebarItems.push_back(item);
  m_content->addChild(item);
}

void SettingsSidebar::updateLayout() {
  m_content->updateLayout();
}

GDL_NS_END
