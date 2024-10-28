#include "SettingsButton.hpp"
#include "../../graphics/backgrounds/Triangles.hpp"

bool SettingsButton::init(std::string label, ccColor3B color) {
  if (!ClickableContainer::init("button-select.wav"_spr)) return false;
  setColor(color);
  setOpacity(255);
  setRadius(10);
  setContentSizeWithUnit({80,40}, Unit::Percent, Unit::UIKit);
  setPosition({20,10});
  addChild(Triangles::create(10,color));
  return true;
}
