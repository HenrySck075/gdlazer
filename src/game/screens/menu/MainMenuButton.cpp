#include "MainMenuButton.hpp" 
#include "../../../frameworks/input/events/KeyEvent.hpp"
#include "ButtonConstants.hpp"
#include "../../../frameworks/graphics/containers/ContainerActions.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"

GDL_NS_START
using namespace frameworks;
bool MainMenuButton::init(std::string text, std::string sampleClick, IconConstructor symbol, Color4 color, ButtonCallback clickAct, std::vector<enumKeyCodes> activa) {
  auto m = CCSize(BUTTON_WIDTH,c_buttonAreaHeight);
  m_color = color;
  ClickableContainer::initWithCallback(sampleClick, clickAct, true);
  ClickableContainer::setContentSize(m);
  //addListener(reactive_listener(updateReactive));

  std::transform(text.begin(), text.end(), text.begin(), [](char in) -> char {
    if (in <= 'Z' && in >= 'A')
      return in - ('Z' - 'z');
    return in;
  });
  activationKeys = activa;

  #define bgSetup(var, id, col)             \
  var = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr); \
  var->setID(id);                             \
  var->setColor(col);                          \
  var->setSkewX(7);                             \
  var->setContentSize(m);                        \
  var->setAnchorPoint(CCPoint(0.5,0.5));          \
  var->setPosition(m/2);                           \

  bgSetup(m_background, "background", color);
  bgSetup(m_hover, "hover", ccc3(255,255,255));
  setColor(Color4{255,255,255,0});
  addChild(m_background);
  addChild(m_hover);
  setCascadeOpacityEnabled(true);
  m_hover->setOpacity(0);
  
  m_icon = symbol;
  m_icon->setScale(0.62);

  auto iconContainer = Container::create();
  iconContainer->setAnchor(Anchor::Center);
  iconContainer->setAnchorPoint({0.5,0.5});
  iconContainer->addChild(m_icon);
  addChild(iconContainer);
  
  m_label = $verifyPtr(OsuText::create(text.c_str(), FontType::Regular, 5));
  m_label->setAnchor(Anchor::Bottom);
  m_label->setAnchorPoint({0.5,0});
  m_label->setPositionY(4, Unit::UIKit);
  addChild(m_label);
  
  
  setAnchorPoint({0.5,0.5});
  addListener<NodeLayoutUpdated>([this](NodeLayoutUpdated*) {
    
    auto s = getContentSize();
    // todo: move to container
    if (m_background) {
      m_background->setContentSize(s);
      m_background->setPosition(s/2);
    }
    if (m_hover) {
      m_hover->setContentSize(s);
      m_hover->setPosition(s/2);
    }
    if (m_askForUpdate) m_pParent->updateLayout();
    return true;
  });
  addListener<KeyEvent>([this](KeyEvent* e){
    if (
      e->m_pressed &&
      std::find(activationKeys.begin(), activationKeys.end(), e->m_key) != activationKeys.end()
    ) {
      click();
    }
    return true;
  });
  addListener<MouseEvent>([this](MouseEvent* e){
    //
    switch (e->m_eventType) {
      case MouseEventType::Enter: 
        runAction(CCEaseElasticOut::create(
          ContainerResizeTo::create(0.5,BUTTON_WIDTH*1.5,c_buttonAreaHeight)
        ));
        FMODAudioEngine::sharedEngine()->playEffect("button-hover.wav"_spr);
        break;
      case MouseEventType::Exit:
        runAction(CCEaseElasticOut::create(
          ContainerResizeTo::create(0.5,BUTTON_WIDTH,c_buttonAreaHeight)
        ));
        break;
      case MouseEventType::MouseDown:
        m_hover->runAction(frameworks::ActionEase::create(
          CCFadeTo::create(1,255*0.1), Easing::OutQuint
        ));
        break;
      case MouseEventType::MouseUp:
        //if (!static_cast<CCBool*>(getUserObject("clicking"_spr))) return;
        m_hover->runAction(frameworks::ActionEase::create(
          CCFadeTo::create(1,0), Easing::OutQuint
        ));
      case MouseEventType::Click:
        m_hover->stopAllActions();
        m_hover->setOpacity(255*0.9);
        m_hover->runAction(frameworks::ActionEase::create(
          CCFadeTo::create(0.8,0), Easing::OutExpo
        ));
    }
    return true;
  });
  return true;
}


GDL_NS_END