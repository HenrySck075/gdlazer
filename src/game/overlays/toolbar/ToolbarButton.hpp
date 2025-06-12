#pragma once

#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"
#include "../../../frameworks/graphics/sprites/CCResizableSprite.hpp"
#include "../../../frameworks/graphics/sprites/IconConstructor.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../graphics/ui/OsuText.hpp"

GDL_NS_START
class ToolbarButton : public frameworks::ClickableContainer {
protected:
  geode::Ref<frameworks::CCResizableSprite> m_iconSprite;
  geode::Ref<frameworks::Container> m_bg;
  geode::Ref<frameworks::Container> m_flashBg;

  geode::Ref<frameworks::FillFlowContainer> m_tooltipContainer; 
  OsuText* m_text;
  OsuText* m_subtext;

public:
  bool init(frameworks::IconConstructor icon, std::string text, std::string sub, AxisAlignment align = AxisAlignment::Start);
  void setTooltipAlignment(AxisAlignment align);
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