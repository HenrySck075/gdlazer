#include "MainMenuButton.hpp" 
#include "../../../frameworks/input/events/KeyEvent.hpp"
#include "ButtonConstants.hpp"
#include "../../../helpers/CustomActions.hpp"
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
  m_body = CCNodeRGBA::create();
  m_body->setContentSize({20,120});
  m_body->setID("ui");
  m_body->setLayout(ColumnLayout::create()->setGap(8)->setAutoScale(false)->setAxisReverse(true));
  m_body->setAnchorPoint({0.5,0.5});
  m_body->setPosition(m/2);
  
  m_icon = symbol;
  m_icon->setScale(0.62);
  m_body->addChild(m_icon);
  
  m_label = OsuText::create(text.c_str(), FontType::Regular, 5);
  if (m_label == nullptr) return false;
  m_body->addChild(m_label);

  m_body->updateLayout();
  
  #define bgSetup(var, id, col)                       \
  var = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr); \
  var->setID(id);                             \
  var->setColor(col);                           \
  var->setSkewX(7);                             \
  var->setContentSize(m);                         \
  var->setAnchorPoint(CCPoint(0.5,0.5));                  \
  var->setPosition(m/2);                          \

  bgSetup(m_background, "background", color);
  bgSetup(m_hover, "hover", ccc3(255,255,255));
  setColor(Color4{255,255,255,0});
  addChild(m_background);
  addChild(m_hover);
  addChild(m_body);
  setCascadeOpacityEnabled(true);
  m_hover->setOpacity(0);
  
  setAnchorPoint({0.5,0.5});

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
    //geode::log::debug("[MainMenuButton]: {}", (int)(e->m_eventType));
    switch (e->m_eventType) {
      case MouseEventType::Enter: 
        runAction(CCEaseElasticOut::create(
          CCResizeTo::create(0.5,BUTTON_WIDTH*1.5,c_buttonAreaHeight)
        ));
        FMODAudioEngine::sharedEngine()->playEffect("button-hover.wav"_spr);
        break;
      case MouseEventType::Exit:
        runAction(CCEaseElasticOut::create(
          CCResizeTo::create(0.5,BUTTON_WIDTH,c_buttonAreaHeight)
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

void MainMenuButton::setContentSize(const CCSize& size) {
  ClickableContainer::setContentSize(size);
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
  if (auto ui = getChildByID("ui")) ui->setPosition(s/2);
  if (m_askForUpdate) m_pParent->updateLayout();
}

GDL_NS_END