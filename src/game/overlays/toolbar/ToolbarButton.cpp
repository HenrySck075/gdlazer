#include "ToolbarButton.hpp"
#include "ToolbarConstants.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../../frameworks/graphics/containers/ContainerActions.hpp"
#include "../../graphics/OsuIcon.hpp"

using namespace ToolbarConstants;
GDL_NS_START
using namespace frameworks;

bool ToolbarButton::init(IconConstructor icon, std::string label, std::string sub, AxisAlignment tooltipAlignment) {
  //setPadding({3});
  
  if (!ClickableContainer::init("toolbar-select.wav"_spr)) return false;

  setCascadeOpacityEnabled(false);

  bg = Container::create();
  bg->setContentSize({92,92}, Unit::Percent);
  bg->setAnchor(Anchor::Center); bg->setAnchorPoint({0.5,0.5});
  bg->setBorderRadius(4);
  bg->setClippingEnabled(true);
  //bg->updateContainerBox(true);
  bg->setBackgroundColor(OsuColor::Gray(80));
  bg->setOpacity(255);

  flashBg = Container::create();
  flashBg->setContentSize({92,92}, Unit::Percent);
  flashBg->setAnchor(Anchor::Center); flashBg->setAnchorPoint({0.5,0.5});
  flashBg->setBorderRadius(4);
  flashBg->setClippingEnabled(true);
  flashBg->updateContainerBox(true);
  flashBg->setBackgroundColor(Color4::White);
  flashBg->setOpacity(255);

  addChild(bg,-7);
  addChild(flashBg, -6);

  tooltipContainer = CCLayerRGBA::create();
  tooltipContainer->setAnchorPoint({
    (float)(int)(tooltipAlignment==AxisAlignment::End),
    1 
  });
  tooltipContainer->setContentHeight(ToolbarConstants::c_tooltipHeight);
  text = OsuText::create(label.c_str(), FontType::Bold, 10);
  subtext = OsuText::create(sub.c_str(), FontType::Regular, 10);
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
  addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated* e){
    iconSprite->setPosition(CCNode::getContentSize()/2);
    if (static_cast<AxisLayout*>(tooltipContainer->getLayout())->getCrossAxisLineAlignment()==AxisAlignment::End) {
      tooltipContainer->setPosition({CCNode::getContentSize().width,0});
    }
    return true;
  });

  addListener<MouseEvent>([this](MouseEvent* e){
    switch (e->m_eventType) {
      case MouseEventType::Enter:
        log::debug("[ToolbarButton]: event id {}", (int)(e->m_eventType));
        bg->stopAllActions();
        bg->runAction(ContainerTintOpacityTo::create(0.2, 180));
        tooltipContainer->stopAllActions();
        tooltipContainer->runAction(CCFadeTo::create(0.2, 255));
        break;
      case MouseEventType::Exit:
        bg->stopAllActions();
        bg->runAction(ContainerTintOpacityTo::create(0.2, 0));
        tooltipContainer->stopAllActions();
        tooltipContainer->runAction(CCFadeTo::create(0.2, 0));
        break;
      case MouseEventType::Click:
        flashBg->runAction(CCSequence::createWithTwoActions(
          ContainerTintOpacityTo::create(0.05,100),
          ContainerTintOpacityTo::create(0.8,0)
        ));
    };
    return true;
  });
  
  setContentSize({c_height,c_height},Unit::UIKit,Unit::UIKit);
  addChild(iconSprite);

  return true;
}

GDL_NS_END