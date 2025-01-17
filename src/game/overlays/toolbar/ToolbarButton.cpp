#include "ToolbarButton.hpp"
#include "ToolbarConstants.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../graphics/ui/OsuText.hpp"

using namespace ToolbarConstants;

bool ToolbarButton::init(IconConstructor icon, std::string label, std::string sub, AxisAlignment tooltipAlignment) {
  setPadding({3});
  bgWrapper = CCClippingNode::create(CCScale9Sprite::createWithSpriteFrameName("roundborder.png"_spr));
  if (bgWrapper->getStencil()==nullptr) return false;
  bgWrapper->getStencil()->setAnchorPoint({0,0});
  bgWrapper->setAlphaThreshold(0.03f);
  ClickableContainer::init("toolbar-select.wav"_spr);
  addListener("nodeLayoutUpdate", [this](NodeEvent* e){
    bgWrapper->setContentSize(CCNode::getContentSize());
    bgWrapper->getStencil()->setContentSize(CCNode::getContentSize());
    e->stopPropagation();
  });
  setCascadeOpacityEnabled(false);

  bg = Container::create();
  bg->setColor(OsuColor::Gray(80));
  bg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
  bg->setOpacity(0);
  flashBg = Container::create();
  flashBg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
  flashBg->setOpacity(0);
  bgWrapper->addChild(bg,-7);
  bgWrapper->addChild(flashBg);
  addChild(bgWrapper);

  tooltipContainer = CCLayerRGBA::create();
  tooltipContainer->setAnchorPoint({
    (float)(int)(tooltipAlignment==AxisAlignment::End),
    1 
  });
  tooltipContainer->setContentHeight(ToolbarConstants::TOOLTIP_HEIGHT);
  text = OsuText(label.c_str(), FontType::Bold, 10);
  subtext = OsuText(sub.c_str(), FontType::Regular, 10);
  tooltipContainer->addChild(text);
  tooltipContainer->addChild(subtext);
  tooltipContainer->setLayout(
    ColumnLayout::create()
    ->setGap(2)
    ->setAutoScale(false)
    ->setAxisReverse(true)
    ->setCrossAxisLineAlignment(tooltipAlignment)
  );
  tooltipContainer->setCascadeOpacityEnabled(true);
  tooltipContainer->setOpacity(0);
  addChild(tooltipContainer);

  iconSprite = icon;
  iconSprite->setScale(0.3);
  addListener("nodeLayoutUpdate",[this](NodeEvent* e){
    iconSprite->setPosition(CCNode::getContentSize()/2);
    if (static_cast<AxisLayout*>(tooltipContainer->getLayout())->getCrossAxisLineAlignment()==AxisAlignment::End) {
      tooltipContainer->setPosition({CCNode::getContentSize().width,0});
    }
  });
  
  setContentSizeWithUnit(CCSize(HEIGHT,HEIGHT),Unit::UIKit,Unit::UIKit);
  addChild(iconSprite);

  return true;
}

void ToolbarButton::onMouseEnter() {
  bg->stopAllActions();
  bg->runAction(CCFadeTo::create(0.2, 180));
  tooltipContainer->stopAllActions();
  tooltipContainer->runAction(CCFadeTo::create(0.2, 255));
}
void ToolbarButton::onMouseExit() {
  bg->stopAllActions();
  bg->runAction(CCFadeTo::create(0.2, 0));
  tooltipContainer->stopAllActions();
  tooltipContainer->runAction(CCFadeTo::create(0.2, 0));
}
void ToolbarButton::onClick(MouseEvent* e) {
  ClickableContainer::onClick(e);
  flashBg->runAction(CCSequence::createWithTwoActions(
    CCFadeTo::create(0.05,100),
    CCFadeTo::create(0.8,0)
  ));
};
