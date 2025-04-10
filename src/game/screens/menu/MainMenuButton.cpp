#include "MainMenuButton.hpp" 
#include "../../graphics/ui/OsuText.hpp"
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
  auto the = CCLayerRGBA::create();
  the->setCascadeOpacityEnabled(true);
  the->setID("ui");
  CCResizableSprite* icon = symbol;
  icon->setScale(0.62);
  the->setLayout(ColumnLayout::create()->setGap(3)->setAutoScale(false)->setAxisReverse(true));
  the->addChild(icon);
  the->setAnchorPoint({0.5,0.5});
  the->setPosition(m/2);
  auto label = OsuText::create(text.c_str(), FontType::Regular);
  label->setScale(0.4);
  the->addChild(label);
  the->updateLayout();
  
  #define bgSetup(var, id, col)                       \
  var = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr); \
  var->setID(id);                             \
  var->setColor(col);                           \
  var->setSkewX(7);                             \
  var->setContentSize(m);                         \
  var->setAnchorPoint(CCPoint(0.5,0.5));                  \
  var->setPosition(m/2);                          \

  bgSetup(background, "background", color);
  bgSetup(hover, "hover", ccc3(255,255,255));
  setCascadeOpacityEnabled(true);
  setColor(Color4{255,255,255,0});
  addChild(background);
  addChild(hover);
  addChild(the);
  hover->setOpacity(0);
  hover->setOpacity(0);
  hover->setOpacity(0);
  hover->setOpacity(0);
  hover->setOpacity(0);
  
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
        hover->runAction(easingsActions::CCEaseOut::create(
          CCFadeTo::create(1,255*0.1),5
        ));
      case MouseEventType::MouseUp:
        //if (!static_cast<CCBool*>(getUserObject("clicking"_spr))) return;
        hover->runAction(easingsActions::CCEaseOut::create(
          CCFadeTo::create(1,0),5
        ));
      case MouseEventType::Click:
        hover->stopAllActions();
        hover->setOpacity(255*0.9);
        hover->runAction(easingsActions::CCEaseExponentialOut::create(
          CCFadeTo::create(0.8,0)
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
  if (background) {
    background->setContentSize(s);
    background->setPosition(s/2);
  }
  if (hover) {
    hover->setContentSize(s);
    hover->setPosition(s/2);
  }
  if (auto ui = getChildByID("ui")) ui->setPosition(s/2);
  if (m_askForUpdate) m_pParent->updateLayout();
}

GDL_NS_END