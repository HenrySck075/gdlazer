#include "SidebarButton.hpp"

GDL_NS_START

bool SidebarButton::init(GDF_KWARGS) {
  if (!OsuButton::init({
    .backgroundColor = args.colorProvider->Background5()
  })) return false;
  
  m_hoverContainer->setBackgroundColor(args.colorProvider->Light4());

  return true;
}

GDL_NS_END