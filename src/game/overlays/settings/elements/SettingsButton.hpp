#pragma once

#include "../../../../frameworks/graphics/containers/ClickableContainer.hpp"

GDL_NS_START

class SettingsButton : public frameworks::ClickableContainer{
  bool setup(std::string label, ccColor3B color);
public:
  bool init(std::string label, ccColor3B color);
  bool initWithCallback(std::string label, ccColor3B color, frameworks::ButtonCallback clickCb);
  static SettingsButton* create(std::string label, ccColor3B color) {
    $createClass(SettingsButton, init, label, color);
  }
  static SettingsButton* create(std::string label, ccColor3B color, frameworks::ButtonCallback clickCb) {
    $createClass(SettingsButton, initWithCallback, label, color, clickCb);
  }
};

GDL_NS_END
