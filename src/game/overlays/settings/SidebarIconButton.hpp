#include "SidebarButton.hpp"
#include "../../../frameworks/graphics/sprites/CCResizableSprite.hpp"

GDL_NS_START

class SidebarIconButton : public SidebarButton {
  $kwargsStruct(
    std::string, header, ,
    frameworks::CCResizableSprite*, icon, ,
    OverlayColorProvider*, provider, 
  )
public:
  static SidebarIconButton* create(GDF_KWARGS) {
    $createClass2(SidebarIconButton);
  }
  bool init(GDF_KWARGS);
};

GDL_NS_END