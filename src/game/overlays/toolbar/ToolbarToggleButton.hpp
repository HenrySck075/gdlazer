/// @note ToolbarOverlayToggleButton

#pragma once

#include "ToolbarButton.hpp"

class ToolbarToggleButton : public ToolbarButton {
  Container* toggleBg;
  bool toggled = false;
public:
  bool init(IconConstructor icon, std::string text, std::string sub, AxisAlignment align = AxisAlignment::Start);
  static ToolbarToggleButton* create(IconConstructor icon, std::string text, std::string sub, ccColor3B stateColor, AxisAlignment align = AxisAlignment::Start) {
    create_class(ToolbarToggleButton, init, icon, text, sub, align);
  };
  void onClick(MouseEvent* e) override;

  virtual void select() {
    toggleBg->setOpacity(255);
    toggled = true;
  };
  virtual void deselect() {
    toggleBg->setOpacity(0);
    toggled = false;
  };
};
