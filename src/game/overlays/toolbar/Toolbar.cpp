#include "Toolbar.hpp"
#include "../../graphics/OsuColor.hpp"
#include "ToolbarButtons.hpp"
#include "../../../frameworks/graphics/animations/ActionEase.hpp"
#include "../../../frameworks/graphics/animations/ContainerActions.hpp"

using namespace ToolbarConstants;
GDL_NS_START
using namespace frameworks;

bool Toolbar::init() {
  if (!Container::init()) return false;
  setOpacity(255);
  setBackgroundColor(OsuColor::Gray(0.1f*255));
  setContentSize({1,c_height},Unit::Viewport,Unit::UIKit);
  setAnchorPoint({0,1});
  setAnchor(Anchor::TopLeft);
  setPosition({0, -c_height},Unit::OpenGL,Unit::UIKit);

  //auto j = processUnit(TOOLTIP_c_height,Unit::UIKit,false);
  auto j = c_tooltipHeight;
  m_gradient = $verifyPtr(CCLayerGradient2::create({0,0,0,0},{0,0,0,0},{0,1},CCLG2Target::Start));
  m_gradient->setAnchorPoint({0,1});// keeping it at the bottom
  m_gradient->ignoreAnchorPointForPosition(false);
  m_gradient->setPosition({0,-j});
  m_gradient->setScaleY(-1);
  addChild(m_gradient);

  m_leftSide = FillFlowContainer::create(FillDirection::Horizontal);
  m_leftSide->setGap(0.5);
  m_leftSide->addChild($verifyPtr(ToolbarSettingsButton::create()));
  m_leftSide->addChild($verifyPtr(ToolbarHomeButton::create()));
  m_leftSide->setAnchor(Anchor::Left);
  m_leftSide->setAnchorPoint({0,0.5});
  m_leftSide->setPosition({
    #ifdef GEODE_IS_ANDROID
    10
    #else
    2
    #endif
    ,0
  });

  m_rightSide = FillFlowContainer::create(FillDirection::Horizontal);
  m_rightSide->setGap(0.5);
  m_rightSide->addChild(ToolbarMusicButton::create());
  m_rightSide->addChild(ToolbarNativeSettingsButton::create());
  m_rightSide->addChild(ToolbarGeodeButton::create());
  /// The last 3 buttons
  m_rightSide->addChild(ToolbarUserButton::create());
  m_rightSide->setAnchor(Anchor::Right);
  m_rightSide->setAnchorPoint({1,0.5});
  m_rightSide->setPosition({
    #ifdef GEODE_IS_ANDROID
    10
    #else
    2
    #endif
    ,0
  });
  //right->addChild(ToolbarModDisableButton::create());

#ifdef GEODE_IS_ANDROID
  left->setPositionX(10);
#endif
  addChild(m_leftSide);
  addChild(m_rightSide);
  setOpacity(255);

  addListener<NodeSizeUpdated>([this,j](NodeSizeUpdated*e){
    m_gradient->setContentSize({CCNode::getContentSize().width,j});
    return true;
  });

  addListener<MouseEvent>([this](MouseEvent* e){
    /*
    if (
      // the one that sends as click is a different event
      e->m_eventType == MouseEventType::MouseDown &&
      isMouseEntered()
    ) e->preventDefault();
     */
    if (e->m_eventType == MouseEventType::Enter) {
      if (!m_shown) return true;
      m_gradient->stopAllActions();
      m_gradient->runAction(frameworks::ActionEase::create(
        CCFadeTo::create(2.5,255), Easing::OutQuint
      ));
    }
    
    if (e->m_eventType == MouseEventType::Exit) {
      if (!m_shown) return true;
      m_gradient->stopAllActions();
      m_gradient->runAction(frameworks::ActionEase::create(
        CCFadeTo::create(0.2,0), Easing::OutQuint
      ));
    }
    return true;
  });

  return true;
}

void Toolbar::show() {
  if (!m_shown) {
    runAction(frameworks::ActionEase::create(
      ContainerMoveTo::create(0.5,{0,0}), Easing::OutQuint
    ));
    m_leftSide->updateLayout();    
    m_rightSide->updateLayout();    
  }
  VisibilityContainer::show();
}

void Toolbar::hide() {
  if (m_shown) {
    runAction(frameworks::ActionEase::create(
      ContainerMoveTo::create(0.5,{0,-c_height}), Easing::OutQuint
    ));
    m_leftSide->updateLayout();    
    m_rightSide->updateLayout();
  }
  VisibilityContainer::hide();
}

GDL_NS_END
