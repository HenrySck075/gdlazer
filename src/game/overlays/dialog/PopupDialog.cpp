#include "PopupDialog.hpp"
#include "../../graphics/backgrounds/Triangles.hpp"
#include "../../../frameworks/graphics/containers/FillFlowContainer.hpp"
#include "../../../frameworks/graphics/CCEase2.hpp"

#include "../../../helpers/colors.hpp"
#include "../../../utils.hpp"
#include "../../../frameworks/bindables/EventTarget.hpp"

GDL_NS_START
using namespace frameworks;

float PopupDialog::width = 250.f;
float PopupDialog::height = 230.f;

bool PopupDialog::init(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons) {
  if (!OsuOverlayContainer::init()) return false;
  CCSize size = CCSize{250,230};
  m_main->setContentSize(size);
  // sizes the dialog overlay to fullscreen aka not counting the toolbar height
  setContentSize({1,1},Unit::Viewport);
  setUserObject("popupdialog"_spr, CCBool::create(true));

  auto winSize = CCDirector::sharedDirector()->getWinSize();
  m_buttons = buttons;

  m_bgSprite = CCScale9Sprite::createWithSpriteFrameName("roundborderlarge.png"_spr);
  m_bgSprite->setAnchorPoint({0,0});
  m_bgSprite->setColor(ccc3(33, 26, 32));
  m_bgSprite->setContentSize(size);
  m_main->addChild(m_bgSprite);

  m_bodyLayout = CCLayerRGBA::create();
  m_bodyLayout->setContentSize(size);
  m_bodyLayout->setAnchorPoint({0, 0});
  m_bodyLayout->setPosition(m_bgSprite->getPosition());
  m_bodyLayout->ignoreAnchorPointForPosition(false);
  m_bodyLayout->setZOrder(2);
  m_bodyLayout->setLayout(
    ColumnLayout::create()
    ->setAutoScale(false)
    ->setGap(2)
    ->setAxisReverse(true)
    ->setCrossAxisOverflow(false)
  );
  m_bodyLayout->setCascadeOpacityEnabled(true);
  
  m_main->addChild(m_bodyLayout);

  //auto batchNode = getChildOfType<CCSpriteBatchNode>(main,0);
  m_bgSpriteClip = CCClippingNode::create(CCScale9Sprite::createWithSpriteFrameName("roundborderlarge.png"_spr));
  m_bgSpriteClip->setAlphaThreshold(0.02f);
  m_bgSpriteClip->setPosition(m_bgSprite->getPosition());
  m_bgSpriteClip->setContentSize(size);
  m_bgSpriteClip->getStencil()->setContentSize(size);
  m_bgSpriteClip->getStencil()->setAnchorPoint({0,0});
  m_bgSpriteClip->setAnchorPoint({0,0}); // in case inherited dialogs also modifies this (who will do this)
  //m_bgSprite->removeFromParent();
  m_main->addChild(m_bgSpriteClip);

  m_bgSpriteClip->addChild(Triangles::create(45,ccc3(30,23,30)));
  //m_title->limitLabelWidth(size.width - 2.f, 1.f, .1f);
  
  m_title = OsuText::create(title.c_str(),FontType::Bold, 18, kCCTextAlignmentCenter);
  m_title->setPosition({size.width / 2, size.height - 71});

  auto label = OsuText::create(content.c_str());
  label->setPosition(m_title->getPosition()-CCPoint{0,12});
  label->setScale(0.4);
  label->setZOrder(0);
  m_title->setZOrder(-1);
  m_title->setScale(0.6);

  m_bodyLayout->addChild(m_title);
  m_bodyLayout->addChild(label);

  auto btnLayer = FillFlowContainer::create(FillDirection::Vertical);
  /*
  btnLayer->setLayout(
    ColumnLayout::create()
    ->setAutoScale(false)
    ->setAxisReverse(true)
    ->setGap(0)
  );
  */
  btnLayer->setAnchorPoint({0.5, 0.5});
  btnLayer->setPosition(size/2);
  btnLayer->setContentSize(size);
  btnLayer->setCascadeOpacityEnabled(true);
  m_main->addChild(btnLayer);

  for (auto& btn : buttons) { 
    btn->setTouchEnabled(false);
    btnLayer->addChild(btn); 
  }
  btnLayer->setID("buttonLayer");
  btnLayer->updateLayout();

  m_bodyLayout->updateLayout();
  //label->limitLabelWidth(size.width - 2.f, 0.4f, .1f);

  // block keyboard inputs
  addListener<KeyEvent>([](KeyEvent* e){e->preventDefault();return true;});
  return true;
}

void PopupDialog::onOpen() {
  m_main->stopAllActions();
  m_main->setScale(0.7);
  m_bgSprite->setOpacity(0);

  m_main->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.75, 1), 0.5));
  m_bgSprite->runAction(easingsActions::CCEaseOut::create(CCFadeIn::create(0.2), 5));

  for (auto* btn : CCArrayExt<PopupDialogButton*>(m_main->getChildByID("buttonLayer")->getChildren())) {
    btn->runAction(CCSequence::createWithTwoActions(
      CCDelayTime::create(0.75),
      CCCallFuncL::create([btn]() {
        btn->setTouchEnabled(true);
      })
    ));
  }
  auto en = FMODAudioEngine::sharedEngine();
  en->playEffect("dialog-pop-in.wav"_spr);
  volume = en->getBackgroundMusicVolume();
  en->setBackgroundMusicVolume(volume*0.3);
}

void PopupDialog::onClose() {
  if (hiding) return;
  hiding = true;
  //setKeypadEnabled(false);
  //setTouchEnabled(false);
  for (auto* btn : CCArrayExt<PopupDialogButton*>(m_main->getChildByID("buttonLayer")->getChildren())) {
    btn->setTouchEnabled(false);
  }
  for (auto* obj : CCArrayExt<CCNode*>(m_main->getChildren())) {
    obj->runAction(easingsActions::CCEaseOut::create(CCFadeOut::create(0.4), 5));
  }
  m_main->runAction(CCEaseOut::create(CCScaleTo::create(0.5, 0.7f), 2));
  m_bgSpriteClip->getChildByType<Triangles>(0)->runAction(easingsActions::CCEaseOut::create(CCFadeOut::create(0.4), 5));
  
  auto en = FMODAudioEngine::sharedEngine();
  en->playEffect("dialog-pop-out.wav"_spr);
  en->setBackgroundMusicVolume(volume);
  runAction(CCSequence::createWithTwoActions(
    CCDelayTime::create(0.5), 
    CCRemoveSelf::create()
  ));
}

bool PopupDialog::init2(
    std::string const &title, std::string const &content,
    std::string const &confirmButtonText, std::string const &cancelButtonText,
    frameworks::ClickableContainer::ButtonCallback confirmCallback) {
  return init(
      title, content,
      {PopupDialogButton::create(confirmButtonText.c_str(),
                                 dialog_button_primary,
                                 "dialog-ok-select.wav"_spr,
                                 [this, confirmCallback](Container *the) {
                                   confirmCallback(the);
                                   hide();
                                 }),
       PopupDialogButton::create(
           cancelButtonText.c_str(), dialog_button_secondary,
           "dialog-cancel-select.wav"_spr, [this](CCNode *self) { hide(); })});
}

GDL_NS_END
