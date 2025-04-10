#include "SettingsButton.hpp"
#include "../../graphics/backgrounds/Triangles.hpp"
#include "../../graphics/ui/OsuText.hpp"

bool SettingsButton::initWithCallback(std::string label, ccColor3B color, ButtonCallback clickCb) {
  setCallback(clickCb);
  return init(label, color);
}

bool SettingsButton::init(std::string label, ccColor3B color) {
  if (!ClickableContainer::init("button-select.wav"_spr)) return false;
  setColor(color);
  setOpacity(255);
  setRadius(50);
  setContentSizeWithUnit({80,40}, Unit::Percent, Unit::UIKit);
  setPosition({20,10});
  addChild(Triangles::create(10,color));
  auto yap = OsuText(label.c_str(),FontType::Regular,12,kCCTextAlignmentCenter);
  yap->setAnchorPoint({0.5,0.5});
  // still soon enough
  addListener("nodeLayoutUpdate",[yap, this](NodeEvent* ){
    yap->setPosition(CCNode::getContentSize()/2);
  });
  addChild(yap);
  return true;
}
