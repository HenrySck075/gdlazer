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

  m_bg = Container::create();
  m_bg->setContentSize({92,92}, Unit::Percent);
  m_bg->setAnchor(Anchor::Center); m_bg->setAnchorPoint({0.5,0.5});
  m_bg->setBorderRadius(4);
  m_bg->setClippingEnabled(true);
  //bg->updateContainerBox(true);
  m_bg->setBackgroundColor(OsuColor::Gray(80).opacity(0));
  m_bg->setOpacity(255);

  m_flashBg = Container::create();
  m_flashBg->setContentSize({92,92}, Unit::Percent);
  m_flashBg->setAnchor(Anchor::Center); m_flashBg->setAnchorPoint({0.5,0.5});
  m_flashBg->setBorderRadius(4);
  m_flashBg->setClippingEnabled(true);
  //flashBg->updateContainerBox(true);
  m_flashBg->setBackgroundColor(Color4::White.opacity(0));
  m_flashBg->setOpacity(255);

  addChild(m_bg,-7);
  addChild(m_flashBg, -5);

  m_tooltipContainer = FillFlowContainer::create(FillDirection::Vertical);
  m_tooltipContainer->setGap(2);
  m_tooltipContainer->setAnchorPoint({
    (float)(int)(tooltipAlignment==AxisAlignment::End),
    1 
  });
  m_tooltipContainer->setContentHeight(ToolbarConstants::c_tooltipHeight);
  m_text = OsuText::create(label.c_str(), FontType::Bold, 8);
  m_subtext = OsuText::create(sub.c_str(), FontType::Regular, 8);
  m_tooltipContainer->addChild(m_subtext);
  m_tooltipContainer->addChild(m_text);
  /*
  tooltipContainer->setLayout(
    ColumnLayout::create()
    ->setGap(2)
    ->setAutoScale(false)
    ->setAxisReverse(true)
    ->setCrossAxisLineAlignment(tooltipAlignment)
  );
  */
  m_tooltipContainer->setCascadeOpacityEnabled(true);
  m_tooltipContainer->setOpacity(0);
  addChild(m_tooltipContainer);
  m_tooltipContainer->updateLayout();
  m_tooltipContainer->setAutoUpdateLayout(false);
  queueInMainThread([this, tooltipAlignment]{setTooltipAlignment(tooltipAlignment);});

  m_iconSprite = icon;
  m_iconSprite->setScale(0.3);
  addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated* e){
    m_iconSprite->setPosition(CCNode::getContentSize()/2);
    return true;
  });

  addListener<MouseEvent>([this](MouseEvent* e){
    switch (e->m_eventType) {
      case MouseEventType::Enter:
        
        m_bg->stopAllActions();
        m_bg->runAction(ContainerTintOpacityTo::create(0.2, 180));
        m_tooltipContainer->stopAllActions();
        m_tooltipContainer->runAction(CCFadeTo::create(0.2, 255));
        break;
      case MouseEventType::Exit:
        m_bg->stopAllActions();
        m_bg->runAction(ContainerTintOpacityTo::create(0.2, 0));
        m_tooltipContainer->stopAllActions();
        m_tooltipContainer->runAction(CCFadeTo::create(0.2, 0));
        break;
      case MouseEventType::Click:
        m_flashBg->runAction(CCSequence::createWithTwoActions(
          ContainerTintOpacityTo::create(0.05,100),
          ContainerTintOpacityTo::create(0.8,0)
        ));
    };
    return true;
  });
  
  setContentSize({c_height,c_height},Unit::UIKit,Unit::UIKit);
  addChild(m_iconSprite);
  

  return true;
}

void ToolbarButton::setTooltipAlignment(AxisAlignment align) {
  // static_cast<RowLayout*>(m_tooltipContainer->getLayout())->setAxisAlignment(align);
  switch (align) {
  case AxisAlignment::End:
    m_tooltipContainer->setChildAnchor(Anchor::BottomRight);
    m_tooltipContainer->setAnchorPoint({1, 1});
    m_tooltipContainer->setAnchor(Anchor::BottomRight);
    break;
  case AxisAlignment::Start:
  default:
    m_tooltipContainer->setChildAnchor(Anchor::BottomLeft);
    m_tooltipContainer->setAnchorPoint({0, 1});
    m_tooltipContainer->setAnchor(Anchor::BottomLeft);
    break;
  }
  m_tooltipContainer->updateLayout();
}
GDL_NS_END