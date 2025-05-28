#include "Toolbar.hpp"
#include "../../graphics/OsuColor.hpp"
#include "ToolbarButtons.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"

using namespace ToolbarConstants;
GDL_NS_START
using namespace frameworks;

bool Toolbar::init() {
  if (!Container::init()) return false;
  setAnchor(Anchor::Top);
  auto bgColor = OsuColor::Gray(0.1f*255);
  setContentSize({1,c_height},Unit::Viewport,Unit::UIKit);
  setAnchorPoint({0,1});
  setAnchor(Anchor::Top);
  setPosition({0, c_height},Unit::OpenGL,Unit::UIKit);

  //auto j = processUnit(TOOLTIP_c_height,Unit::UIKit,false);
  auto j = c_tooltipHeight;
  gradient = $verifyPtr(CCLayerGradient2::create({0,0,0,0},{0,0,0,0},{0,1},CCLG2Target::Start));
  gradient->setAnchorPoint({0,1});// keeping it at the bottom
  gradient->ignoreAnchorPointForPosition(false);
  gradient->setPosition({0,-j});
  gradient->setScaleY(-1);
  addChild(gradient);

  auto left = CCLayer::create();
  left->addChild($verifyPtr(ToolbarSettingsButton::create()));
  left->addChild($verifyPtr(ToolbarHomeButton::create()));

  auto lay = 
    RowLayout::create()
    ->setAutoScale(false)
    ->setAxisAlignment(AxisAlignment::Start)
    ->setGap(-0.5);
  // this will cause issues
  left->setLayout(lay);
  left->setAnchorPoint({0,0});

  auto right = CCLayer::create();
  right->addChild(ToolbarMusicButton::create());
  right->addChild(ToolbarGeodeButton::create());
  right->addChild(ToolbarModDisableButton::create());

  // this will also cause issues
  right->setLayout(
    lay
    ->setAxisAlignment(AxisAlignment::End)
  );
  right->setAnchorPoint({1,0});
#ifdef GEODE_IS_ANDROID
  left->setPositionX(10);
#endif

  addChild(left);
  addChild(right);
  setColor(bgColor);
  setOpacity(255);

  addListener<NodeLayoutUpdated>([this,j,right](NodeLayoutUpdated*e){
    gradient->setContentSize({CCNode::getContentSize().width,j});
    right->setPositionX(
      CCNode::getContentSize().width
#ifdef GEODE_IS_ANDROID
      -10
#endif
    );
    return true;
  });

  addListener<MouseEvent>([this](MouseEvent* e){
    if (
      // the one that sends as click is a different event
      e->m_eventType == MouseEventType::MouseDown &&
      isMouseEntered()
    ) e->preventDefault();
    if (e->m_eventType == MouseEventType::Enter) {
      if (!m_shown) return true;
      gradient->stopAllActions();
      gradient->runAction(easingsActions::CCEaseOut::create(CCFadeTo::create(2.5,255), 5));
    }
    
    if (e->m_eventType == MouseEventType::Exit) {
      if (!m_shown) return true;
      gradient->stopAllActions();
      gradient->runAction(easingsActions::CCEaseOut::create(CCFadeTo::create(0.2,0), 5));
    }
    return true;
  });

  return true;
}

void Toolbar::show() {
  if (!m_shown) runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(0.5,{0,0}), 5
  ));
  VisibilityContainer::show();
}

void Toolbar::hide() {
  if (m_shown) runAction(easingsActions::CCEaseOut::create(
    CCMoveTo::create(0.5,{0,c_height}), 5
  ));
  VisibilityContainer::hide();
}

GDL_NS_END
