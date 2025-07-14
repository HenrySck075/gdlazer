#include "SidebarIconButton.hpp"

GDL_NS_START
using namespace frameworks;
bool SidebarIconButton::init(GDF_KWARGS) {
  ctorInitArgs(args);
  if (!SidebarButton::init({.colorProvider = m_provider})) return false;

  m_icon->setAnchorPoint({0.5, 0.5});
  //m_icon->setAnchor();
  addChild(m_icon);

  setContentWidth(100, Unit::Percent);

  return true;
}

GDL_NS_END