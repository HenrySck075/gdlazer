#include "PopupDialogButton.hpp"
#include "../../../utils.hpp"
#include "../../graphics/backgrounds/Triangles.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "../../graphics/ui/OsuText.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"

GDL_NS_START
using namespace frameworks;

PopupDialogButton* PopupDialogButton::create(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb) {
  auto ret = new PopupDialogButton();
  if (ret && ret->init(label, color, clickSfx, clickCb)) {
    ret->autorelease();
  }
  else {
    delete ret;
    ret = nullptr;
  }
  return ret;
}

bool PopupDialogButton::init(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb) {
  m_color = color;
  setZOrder(3);

  auto dialogBg = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr);
  if (dialogBg!=nullptr) {
    dialogBg->setID("dialogbutton-background");
    dialogBg->setAnchorPoint({0.5, 0.5});
    dialogBg->setContentHeight(m_height);
    dialogBg->setColor(color);
    dialogBg->setSkewX(7);
  } else {
    log::debug("what the fuck?");
  }

  auto clipNode = CCClippingNode::create();
  clipNode->setID("dialogbutton-clipnode");
  clipNode->addChild(Triangles::create(4,color));

  auto j = OsuText(label, FontType::Bold);
  j->setID("dialogbutton-label");
  j->setAnchorPoint({0.5, 0.5});
  j->setScale(0.4);

  // Gradient layers
#define gradientSetup(side) \
  m_grad##side = CCLayerGradient::create(ccc4BFromccc4F(ccc4FFromccc3B(color)), ccc4(color.r, color.g, color.b, 0), {1, 0}); \
  m_grad##side->setContentSize({0,m_height}); \
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
  
  ClickableContainer::initWithCallback(clickSfx, clickCb, true);

  addListener<MouseEvent>([this, dialogBg](MouseEvent* e){
    switch (e->m_eventType) {
      case MouseEventType::Enter:
        FMODAudioEngine::sharedEngine()->playEffect("default-hover.wav"_spr);
        dialogBg->runAction(
          easingsActions::CCEaseOut::create(
            CCResizeTo::create(
              0.1f,
              m_pParent->getParent()->getContentWidth()*c_hoverWidth, 
              m_height
            ),
            5
          )
        );
        #define gradAct easingsActions::CCEaseOut::create(CCFadeOut::create(0.1f),5)
        m_gradLeft->runAction(gradAct);
        m_gradRight->runAction(gradAct);
        #undef gradAct
      
      case MouseEventType::Exit:
        dialogBg->runAction(
          easingsActions::CCEaseOut::create(
            CCResizeTo::create(
              0.1f, 
              m_pParent->getParent()->getContentWidth()*c_idleWidth,
              m_height
            ),
            5
          )
        );
        #define gradAct easingsActions::CCEaseOut::create(CCFadeOut::create(0.1f),5)
        m_gradLeft->runAction(gradAct);
        m_gradRight->runAction(gradAct);
        #undef gradAct

      case MouseEventType::MouseDown:
        dialogBg->runAction(
          easingsActions::CCEaseOut::create(
            CCResizeTo::create(
              c_clickDuration*4, 
              m_pParent->getParent()->getContentWidth()*c_hoverWidth*0.98f,
              m_height
            ),
            4
          )
        );


      case MouseEventType::MouseUp: 
        dialogBg->runAction(
          CCEaseIn::create(
            CCResizeTo::create(
              c_clickDuration, 
              m_pParent->getParent()->getContentWidth()*c_hoverWidth,
              m_height
            ),
            1
          )
        );

    }
    return true;
  });

  addListener<NodeLayoutUpdated>([this, j, dialogBg](NodeLayoutUpdated* e){
    auto size = getContentSize();
    j->setPosition(size/2);
    if (dialogBg!=nullptr) {
      dialogBg->setContentWidth(size.width * 0.8);
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

  setMinSize({-1,m_height}), setMaxSize({-1,m_height});

  return true;

}

void PopupDialogButton::setOpacity(GLubyte opacity) {
  CCClippingNodeRGBA::setOpacity(opacity); 
  m_gradLeft->setOpacity(opacity);
  m_gradRight->setOpacity(opacity);
}

/*
void PopupDialogButton::setContentWidth(float width) {
  setContentSize(CCSize{width,getContentHeight()});
}
*/
// no
void PopupDialogButton::setContentHeight(float height) {
}

GDL_NS_END
