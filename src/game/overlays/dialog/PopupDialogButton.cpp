#include "PopupDialogButton.hpp"
#include "../../../utils.hpp"
#include "../../graphics/backgrounds/Triangles.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "../../graphics/ui/OsuText.hpp"
#include <henrysck075.easings/include/easings.hpp>

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

  auto d = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr);
  if (d!=nullptr) {
    d->setID("dialogbutton-background");
    d->setAnchorPoint({0.5, 0.5});
    d->setContentHeight(height);
    d->setColor(color);
    d->setSkewX(7);
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
  auto grad##side = CCLayerGradient::create(ccc4BFromccc4F(ccc4FFromccc3B(color)), ccc4(color.r, color.g, color.b, 0), {1, 0}); \
  grad##side->setContentSize(CCSize(0,height)); \
  grad##side->setID("gradient"#side); \
  grad##side->setOpacity(0); \
  addChild(grad##side)
  
  gradientSetup(Left);
  gradientSetup(Right);
#undef gradientSetup
  gradLeft->setScaleX(-1);

  if (d!=nullptr) addChild(d);
  //addChild(clipNode);
  addChild(j);
  
  ClickableContainer::init(clickSfx, clickCb);
  addListener("nodeLayoutUpdate", [this](NodeEvent* e){
    auto size = getContentSize();
    getChildByID("dialogbutton-label")->setPosition(size/2);
    auto d = getChildByID("dialogbutton-background");
    if (d!=nullptr) {
      d->setContentWidth(size.width * 0.8);
      d->setPosition(size / 2);
    }

    float gradWidth = size.width * 0.125f;
    
    auto gradLeft = static_cast<CCLayerGradient*>(getChildByID("gradientLeft"));
    gradLeft->setContentWidth(gradWidth);
    auto gradRight = static_cast<CCLayerGradient*>(getChildByID("gradientRight"));
    gradRight->setContentWidth(gradWidth);
    gradRight->setPosition({size.width-gradWidth, 0.f});
  });

  setContentSizeWithUnit(CCSize{ 100,height },Unit::Percent,Unit::UIKit);
  setAnchorPoint(CCPoint{ 0.5,0.5 });

  setCascadeOpacityEnabled(true);

  setSizeConstraints({-1,height}, {-1,height});

  return true;

}

void PopupDialogButton::setOpacity(GLubyte opacity) {
  CCLayerRGBA::setOpacity(opacity); 
  static_cast<CCLayerGradient*>(getChildByID("gradientLeft"))->setOpacity(opacity);
  static_cast<CCLayerGradient*>(getChildByID("gradientRight"))->setOpacity(opacity);
}

/*
void PopupDialogButton::setContentWidth(float width) {
  setContentSize(CCSize{width,getContentHeight()});
}
*/
// no
void PopupDialogButton::setContentHeight(float height) {
}


void PopupDialogButton::onMouseEnter() {
  if (!getHoverEnabled()) return;
  FMODAudioEngine::sharedEngine()->playEffect("default-hover.wav"_spr);
  getChildByID("dialogbutton-background")->runAction(
    easingsActions::CCEaseOut::create(
      CCResizeTo::create(
        0.1f,
        m_pParent->getParent()->getContentWidth()*hover_width, 
        height
      ),
      5
    )
  );
  #define gradAct easingsActions::CCEaseOut::create(CCFadeOut::create(0.1f),5)
  getChildByID("gradientLeft")->runAction(gradAct);
  getChildByID("gradientRight")->runAction(gradAct);
  #undef gradAct
}
void PopupDialogButton::onMouseExit() {
  if (!getHoverEnabled()) return;
  getChildByID("dialogbutton-background")->runAction(
    easingsActions::CCEaseOut::create(
      CCResizeTo::create(
        0.1f, 
        m_pParent->getParent()->getContentWidth()*idle_width,
        height
      ),
      5
    )
  );
  #define gradAct easingsActions::CCEaseOut::create(CCFadeOut::create(0.1f),5)
  getChildByID("gradientLeft")->runAction(gradAct);
  getChildByID("gradientRight")->runAction(gradAct);
  #undef gradAct
}
void PopupDialogButton::onMouseDown(MouseEvent* event) {
  getChildByID("dialogbutton-background")->runAction(
    easingsActions::CCEaseOut::create(
      CCResizeTo::create(
        click_duration*4, 
        m_pParent->getParent()->getContentWidth()*hover_width*0.98f,
        height
      ),
      4
    )
  );
}
void PopupDialogButton::onMouseUp(MouseEvent* event) {
  getChildByID("dialogbutton-background")->runAction(
    CCEaseIn::create(
      CCResizeTo::create(
        click_duration, 
        m_pParent->getParent()->getContentWidth()*hover_width,
        height
      ),
      1
    )
  );
}
