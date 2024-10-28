#pragma once

#include "../../../framework/graphics/containers/ClickableContainer.hpp"

class SettingsButton : public ClickableContainer {
public:
  bool init(std::string label, ccColor3B color);
  static SettingsButton* create(std::string label, ccColor3B color) {
    create_class(SettingsButton, init, label, color);
  }
};
