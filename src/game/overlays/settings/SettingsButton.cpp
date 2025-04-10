#include "SettingsButton.hpp"
#include "../../graphics/backgrounds/Triangles.hpp"
#include "../../graphics/ui/OsuText.hpp"

GDL_NS_START
using namespace frameworks;

bool SettingsButton::initWithCallback(std::string label, ccColor3B color, ButtonCallback clickCb) {
  if (!ClickableContainer::initWithCallback("button-select.wav"_spr, clickCb, true)) return false;
  return setup(label, color);
}

bool SettingsButton::init(std::string label, ccColor3B color) {
  if (!ClickableContainer::init("button-select.wav"_spr, true)) return false;
  return setup(label, color);
}
bool SettingsButton::setup(std::string label, ccColor3B color) {
  setColor(color);
  setOpacity(255);
  setContentSize({80,40}, Unit::Percent, Unit::UIKit);
  setPosition({20,10});
  addChild(Triangles::create(10,color));
  auto yap = OsuText(label.c_str(),FontType::Regular,12,kCCTextAlignmentCenter);
  yap->setAnchorPoint({0.5,0.5});
  addChild(yap);
  return true;
}

GDL_NS_END
