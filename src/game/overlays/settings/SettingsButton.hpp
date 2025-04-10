#pragma once

#include "../../../framework/graphics/containers/ClickableContainer.hpp"

class SettingsButton : public ClickableContainer {
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
