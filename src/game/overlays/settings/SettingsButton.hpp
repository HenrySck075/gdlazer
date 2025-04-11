#pragma once

#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"
#include "../../../frameworks/graphics/containers/CircularContainer.hpp"

GDL_NS_START

class SettingsButton : public frameworks::ClickableContainer{
  bool setup(std::string label, ccColor3B color);
public:
  bool init(std::string label, ccColor3B color);
  bool initWithCallback(std::string label, ccColor3B color, ButtonCallback clickCb);
  static SettingsButton* create(std::string label, ccColor3B color) {
    $create_class(SettingsButton, init, label, color);
  }
  static SettingsButton* create(std::string label, ccColor3B color, ButtonCallback clickCb) {
    $create_class(SettingsButton, initWithCallback, label, color, clickCb);
  }
};

GDL_NS_END
