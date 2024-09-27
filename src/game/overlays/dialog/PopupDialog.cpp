#include "PopupDialog.hpp"
#include "../../graphics/backgrounds/Triangles.hpp"
#include "../../../framework/graphics/containers/FillFlowContainer.hpp"
#include "../../graphics/ui/OsuText.hpp"

float PopupDialog::width = 250.f;
float PopupDialog::height = 230.f;

bool PopupDialog::init(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons) {
    if (!OsuOverlayContainer::init()) return false;
    CCSize size {width,height};
    // sizes the dialog overlay to fullscreen aka not counting the toolbar height
    setContentSizeWithUnit({1,1},Unit::Viewport,Unit::Viewport);
    setUserObject("popupdialog"_spr, CCBool::create(true));

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_buttons = buttons;

    m_bodyLayout = CCLayerRGBA::create();
    m_bodyLayout->setContentSize(size);
    m_bodyLayout->setAnchorPoint(ccp(0.5, 0.5));
    m_bodyLayout->setPosition(m_bgSprite->getPosition());
    m_bodyLayout->ignoreAnchorPointForPosition(false);
    m_bodyLayout->setZOrder(2);
    m_bodyLayout->setLayout(ColumnLayout::create()->setAutoScale(false)->setGap(2)->setAxisReverse(true));
    m_bodyLayout->setCascadeOpacityEnabled(true);
    
    main->addChild(m_bodyLayout);

    auto contentSize = size;
    m_bgSprite = CCScale9Sprite::createWithSpriteFrameName("roundborderlarge.png"_spr);
    m_bgSprite->setColor(ccc3(33, 26, 32));
    m_bgSprite->setContentSize(size);
    main->addChild(m_bgSprite);

    //auto batchNode = getChildOfType<CCSpriteBatchNode>(main,0);
    m_bgSpriteClip = CCClippingNode::create(m_bgSprite);
    m_bgSpriteClip->setAlphaThreshold(0.02f);
    m_bgSpriteClip->setPosition(m_bgSprite->getPosition());
    m_bgSpriteClip->setContentSize(contentSize);
    m_bgSpriteClip->setAnchorPoint(m_bgSprite->getAnchorPoint()); // in case inherited dialogs also modifies this (who will do this)
    //m_bgSprite->removeFromParent();
    main->addChild(m_bgSpriteClip);

    m_bgSpriteClip->addChild(Triangles::create(45,ccc3(30,23,30)));
    //m_title->limitLabelWidth(contentSize.width - 2.f, 1.f, .1f);
    
    m_title = OsuText(title.c_str(),FontType::Bold, 18, kCCTextAlignmentCenter);
    m_title->setPosition({size.width / 2, size.height - 71});
    main->addChild(m_title);

    auto label = CCLabelBMFont::create(content.c_str(), "torus-regular.fnt"_spr);
    label->setPosition(m_title->getPosition()-CCPoint{0,12});
    label->setScale(0.4);
    label->setZOrder(0);
    m_title->setZOrder(-1);
    m_title->setScale(0.6);

    m_title->removeFromParent();
    m_bodyLayout->addChild(m_title);
    m_bodyLayout->addChild(label);

    auto btnLayer = CCLayerColor::create();
    btnLayer->setLayout(
        ColumnLayout::create()
        ->setAutoScale(false)
        ->setAxisReverse(true)
        ->setGap(0)
    );
    btnLayer->setAnchorPoint(ccp(0.5, 0.5));
    btnLayer->setPosition(contentSize/2);
    btnLayer->setContentSize(contentSize);
    btnLayer->setCascadeOpacityEnabled(true);
    main->addChild(btnLayer);

    for (auto& btn : buttons) { 
        btn->setHoverEnabled(false);
        btn->setClickEnabled(false);
        btnLayer->addChild(btn); 
    }
    btnLayer->setID("buttonLayer");

    m_bodyLayout->updateLayout();
    label->limitLabelWidth(contentSize.width - 2.f, 0.4f, .1f);

    // block keyboard inputs
    addListener("keyboardEvent", [](NodeEvent* e){e->preventDefault();});
    return true;
}

void PopupDialog::onOpen() {
    main->stopAllActions();
    main->setScale(0.7);
    m_bgSprite->setOpacity(0);

    main->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.75, 1), 0.5));
    m_bgSprite->runAction(CCEaseOutQuint::create(CCFadeIn::create(0.2)));

    for (auto* btn : CCArrayExt<PopupDialogButton*>(main->getChildByID("buttonLayer")->getChildren())) {
        btn->runAction(CCSequence::createWithTwoActions(
            CCDelayTime::create(0.75),
            CCCallFuncL::create([btn]() {
                btn->setHoverEnabled(true);
                btn->setClickEnabled(true);
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
    //this->setKeypadEnabled(false);
    //this->setTouchEnabled(false);
    for (auto* btn : CCArrayExt<PopupDialogButton*>(main->getChildByID("buttonLayer")->getChildren())) {
        btn->setHoverEnabled(false);
        btn->setClickEnabled(false);
    }
    for (auto* obj : CCArrayExt<CCNode*>(main->getChildren())) {
        obj->runAction(CCEaseOutQuint::create(CCFadeOut::create(0.4)));
    }
    main->runAction(CCEaseOut::create(CCScaleTo::create(0.5, 0.7f), 2));
    getChildOfType<Triangles>(m_bgSpriteClip, 0)->runAction(CCEaseOutQuint::create(CCFadeOut::create(0.4)));
    
    auto en = FMODAudioEngine::sharedEngine();
    en->playEffect("dialog-pop-out.wav"_spr);
    en->setBackgroundMusicVolume(volume);
    this->runAction(CCSequence::createWithTwoActions(
        CCDelayTime::create(0.5), 
        CCRemoveSelf::create()
    ));
}

void PopupDialog::keyBackClicked() {
    this->hide();
}
