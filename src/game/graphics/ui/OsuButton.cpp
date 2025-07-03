#include "OsuButton.hpp"
#include "../../../frameworks/graphics/animations/ActionEase.hpp"

GDL_NS_START
using namespace frameworks;
bool OsuButton::init(GDF_KWARGS) {
  if (!ClickableContainer::init("button-select.wav"_spr)) return false;

  setContentHeight(40, Unit::UIKit);
  setBackgroundColor(args.backgroundColor);
  setBorderRadius(5);
  setAnchorPoint({0.5,0.5});

  addChild(m_hoverContainer = frameworks::Container::create());
  m_hoverContainer->setBackgroundColor(Color4::White);
  m_hoverContainer->setContentSize({100,100},Unit::Percent);
  m_hoverContainer->setOpacity(0);

  addChild(m_flashContainer = frameworks::Container::create(), 99999);
  m_flashContainer->setBackgroundColor(Color4::White.opacity(255/2.f));
  m_flashContainer->setContentSize({100,100},Unit::Percent);
  m_flashContainer->setOpacity(0);

  addListener<MouseEvent>([this](MouseEvent* event){
    if (event->m_eventType == MouseEventType::Enter) {
      m_hoverContainer->runAction(CCSequence::createWithTwoActions(
        ActionEase::create(CCFadeTo::create(0.04, 255*0.2), Easing::OutQuint),
        ActionEase::create(CCFadeTo::create(0.8, 255*0.1), Easing::OutQuint)
      ));
    } else if (event->m_eventType == MouseEventType::Exit) {
      m_hoverContainer->runAction(ActionEase::create(CCFadeTo::create(0.8, 0), Easing::OutQuint));
    } else if (event->m_eventType == MouseEventType::MouseDown) {
      m_flashContainer->runAction(ActionEase::create(CCScaleTo::create(4, 0.9), Easing::OutQuint));
    } else if (event->m_eventType == MouseEventType::MouseUp) {
      m_flashContainer->runAction(ActionEase::create(CCScaleTo::create(1, 1), Easing::OutElastic));
    }
    return true;
  });

  return true;
}

GDL_NS_END