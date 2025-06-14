#include "OsuTabControl.hpp"

#include "../../../frameworks/graphics/containers/ContainerActions.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"

GDL_NS_START

bool OsuTabItem::init(std::string text, ccColor4B accent) {
  if (!frameworks::TabItem::init())
    return false;
  setContentHeight(100, frameworks::Unit::Percent);
  m_text = text;
  m_accent = accent;
  m_textNode = OsuText::create(m_text);
  m_textNode->setAnchor(Anchor::Center);
  m_textNode->setAnchorPoint({0.5, 0.5});
  m_textNode->inner()->setColor({accent.r, accent.g, accent.b});
  m_textNode->inner()->setOpacity(accent.a);
  addChild(m_textNode);
  m_bar = Container::create();
  m_bar->setContentSize({100, 3}, frameworks::Unit::Percent,
                        frameworks::Unit::UIKit);
  m_bar->setAnchorPoint({0, 0});
  m_bar->setBackgroundColor({255, 255, 255, 0});
  addChild(m_bar);

  setContentWidth(MIN_WIDTH, frameworks::Unit::UIKit);

  addListener<frameworks::MouseEvent>([this](frameworks::MouseEvent *event) {
    if (event->m_eventType == frameworks::MouseEventType::Enter) {
      fadeIn();
    } else if (event->m_eventType == frameworks::MouseEventType::Exit) {
      fadeOut();
    }
    return true;
  });

  return true;
}
#define $outQuinting(action) frameworks::ActionEase::create(action, frameworks::Easing::OutQuint)
void OsuTabItem::fadeIn() {
  m_bar->runAction(
      $outQuinting(frameworks::ContainerTintOpacityTo::create(0.5, 255)));
  m_textNode->inner()->runAction(
      $outQuinting(CCTintTo::create(0.5, 255, 255, 255)));
  m_textNode->inner()->runAction($outQuinting(CCFadeTo::create(0.5, 255)));
}
void OsuTabItem::fadeOut() {
  if (isMouseEntered())
    return;
  m_bar->runAction(
      $outQuinting(frameworks::ContainerTintOpacityTo::create(0.5, 0)));
  m_textNode->inner()->runAction(
      $outQuinting(CCTintTo::create(0.5, 255, 255, 255)));
  m_textNode->inner()->runAction($outQuinting(CCFadeTo::create(0.5, 255)));
}
#undef $outQuinting

GDL_NS_END