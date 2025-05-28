#pragma once

#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"
#include "../../../frameworks/graphics/sprites/CCResizableSprite.hpp"
#include "../../../frameworks/graphics/sprites/IconConstructor.hpp"
#include "../../graphics/ui/OsuText.hpp"

GDL_NS_START
class ToolbarButton : public frameworks::ClickableContainer {
  frameworks::CCResizableSprite* iconSprite;
  Container* bg;
  Container* flashBg;

  CCLayerRGBA* tooltipContainer; 
  OsuText* text;
  OsuText* subtext;

protected:
  CCClippingNode* bgWrapper;
public:
  bool init(frameworks::IconConstructor icon, std::string text, std::string sub, AxisAlignment align = AxisAlignment::Start);
  void setTooltipAlignment(AxisAlignment align) {
    static_cast<RowLayout*>(tooltipContainer->getLayout())->setAxisAlignment(align);
    switch (align) {
      case AxisAlignment::Start: 
        tooltipContainer->setAnchorPoint({0,1});
        break;
      case AxisAlignment::End: 
        tooltipContainer->setAnchorPoint({1,1});
        break;
      default: log::info("[ToolbarButton]: not supported :(");
    }
    tooltipContainer->updateLayout();
  }
  /**
   * @param icon Toolbar icon
   * @param text Toolbar text
   * @param sub scribe
   */
  static ToolbarButton* create(frameworks::IconConstructor icon, std::string text, std::string sub, AxisAlignment align = AxisAlignment::Start) {
    $createClass(ToolbarButton, init, icon, text, sub, align);
  }
};
GDL_NS_END