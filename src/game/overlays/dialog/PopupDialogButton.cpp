#include "PopupDialogButton.hpp"
#include "../../../utils.hpp"
#include "../../graphics/backgrounds/Triangles.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "../../graphics/ui/OsuText.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"
#include "../../../frameworks/graphics/color/Color4.hpp"

GDL_NS_START
using namespace frameworks;

PopupDialogButton* PopupDialogButton::create(std::string label, ccColor3B color, std::string clickSfx, ButtonCallback clickCb) {
  $createClass(PopupDialogButton, init, label, color, clickSfx, clickCb);
}

bool PopupDialogButton::init(std::string label, ccColor3B color, std::string clickSfx, ButtonCallback clickCb) {
  m_color = color;
  setZOrder(3);

  auto dialogBg = $verifyPtr(CCScale9Sprite::createWithSpriteFrameName("square.png"_spr));
  dialogBg->setID("dialogbutton-background");
  dialogBg->setAnchorPoint({0.5, 0.5});
  dialogBg->setContentHeight(m_height);
  dialogBg->setColor(color);
  dialogBg->setSkewX(7);

  auto clipNode = CCClippingNode::create();
  clipNode->setID("dialogbutton-clipnode");
  clipNode->addChild(Triangles::create(4,color));

  auto j = $verifyPtr(OsuText::create(label, FontType::Bold));
  j->setID("dialogbutton-label");
  j->setAnchorPoint({0.5, 0.5});
  j->setScale(0.4);

  // Gradient layers
#define gradientSetup(side) \
  m_grad##side = CCLayerGradient::create(ccc4BFromccc4F(ccc4FFromccc3B(color)), ccc4(color.r, color.g, color.b, 0), {1, 0}); \
  m_grad##side->setContentSize({0,processUnit(m_height, Unit::UIKit, false)}); \
  m_grad##side->setID("gradient"#side); \
  m_grad##side->setOpacity(0); \
  addChild(m_grad##side)
  
  gradientSetup(Left);
  gradientSetup(Right);
#undef gradientSetup
  m_gradLeft->setScaleX(-1);

  if (dialogBg!=nullptr) addChild(dialogBg);
  //addChild(clipNode);
  addChild(j);

  m_grimace = CCNodeRGBA::create();
  m_grimace->setOpacity(0);
  
  if (!ClickableContainer::initWithCallback(clickSfx, clickCb, true)) return false;

  addListener<MouseEvent>([this, dialogBg](MouseEvent* e){
    float height = getContentHeight();
    switch (e->m_eventType) {
      case MouseEventType::Enter: {
        FMODAudioEngine::sharedEngine()->playEffect("default-hover.wav"_spr);
        dialogBg->runAction(
          frameworks::ActionEase::create(
            CCResizeTo::create(
              0.1f,
              m_pParent->getParent()->getContentWidth()*c_hoverWidth, 
              height
            ),
            Easing::OutQuint
          )
        );
        #define gradAct frameworks::ActionEase::create(CCFadeIn::create(0.1f), Easing::OutQuint)
        m_gradLeft->runAction(gradAct);
        m_gradRight->runAction(gradAct);
        m_grimace->runAction(gradAct);
        #undef gradAct
        break;
      }
      
      case MouseEventType::Exit: {
        dialogBg->runAction(
          frameworks::ActionEase::create(
            CCResizeTo::create(
              0.1f, 
              m_pParent->getParent()->getContentWidth()*c_idleWidth,
              height
            ),
            Easing::OutQuint
          )
        );
        #define gradAct frameworks::ActionEase::create(CCFadeOut::create(0.1f), Easing::OutQuint)
        m_gradLeft->runAction(gradAct);
        m_gradRight->runAction(gradAct);
        m_grimace->runAction(gradAct);
        #undef gradAct
        break;
      }

      case MouseEventType::MouseDown: {
        dialogBg->runAction(
          frameworks::ActionEase::create(
            CCResizeTo::create(
              c_clickDuration*4, 
              m_pParent->getParent()->getContentWidth()*c_hoverWidth*0.98f,
              height
            ),
            Easing::OutQuad
          )
        );
        break;
      }

      case MouseEventType::MouseUp: {
        dialogBg->runAction(
          frameworks::ActionEase::create(
            CCResizeTo::create(
              c_clickDuration, 
              m_pParent->getParent()->getContentWidth()*c_hoverWidth,
              height
            ),
            Easing::In
          )
        );
        break;
      }

    }
    return true;
  });

  addListener<NodeLayoutUpdated>([this, j, dialogBg](NodeLayoutUpdated* e){
    auto size = getContentSize();
    j->setPosition(size/2);
    if (dialogBg!=nullptr) {
      dialogBg->setContentSize({size.width * 0.8f, size.height});
      dialogBg->setPosition(size / 2);
    }

    float gradWidth = size.width * 0.125f;
    
    m_gradLeft->setContentWidth(gradWidth);
    m_gradRight->setContentWidth(gradWidth);
    m_gradRight->setPosition({size.width-gradWidth, 0.f});
    return true;
  });

  setContentSize({ 100,m_height },Unit::Percent,Unit::UIKit);
  setAnchorPoint({ 0.5,0.5 });

  setCascadeOpacityEnabled(true);

  setBackgroundColor(Color4::fromHex("150e14"));

  /*
  setMinSize({-1,m_height-1});
  setMaxSize({-1,m_height});
*/
  return true;
}

/*
void PopupDialogButton::setContentWidth(float width) {
  setContentSize(CCSize{width,getContentHeight()});
}
*/
// no
void PopupDialogButton::setContentHeight(float height) {
}

void PopupDialogButton::updateDisplayedOpacity(GLubyte parentOpacity) {
  frameworks::ClickableContainer::updateDisplayedOpacity(parentOpacity);
  m_gradLeft->setOpacity(m_grimace->getOpacity()*(parentOpacity/255));
  m_gradRight->setOpacity(m_grimace->getOpacity()*(parentOpacity/255));
}
/// Pretend that m_grimace is part of the node tree
void PopupDialogButton::onEnter() {
  frameworks::ClickableContainer::onEnter();
  m_grimace->onEnter();
}

void PopupDialogButton::onExit() {
  frameworks::ClickableContainer::onExit();
  m_grimace->onExit();
}
GDL_NS_END
