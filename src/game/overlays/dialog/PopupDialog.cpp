#include "PopupDialog.hpp"
#include "../../graphics/ui/deco/Triangles.hpp"

bool PopupDialog::setup(std::string const& title, std::string const& content, std::initializer_list<PopupDialogButton*> buttons) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // convenience function provided by Popup
    // for adding/setting a title to the popup
    this->setTitle(title);
    m_bodyLayout = CCLayerRGBA::create();
    m_bodyLayout->setContentSize(m_bgSprite->getContentSize());
    m_bodyLayout->setAnchorPoint(ccp(0.5, 0.5));
    m_bodyLayout->setPosition(m_bgSprite->getPosition());
    m_bodyLayout->ignoreAnchorPointForPosition(false);
    m_bodyLayout->setZOrder(2);
    m_bodyLayout->setLayout(ColumnLayout::create()->setAutoScale(false)->setGap(2)->setAxisReverse(true));
    m_bodyLayout->setCascadeOpacityEnabled(true);
    
    m_mainLayer->addChild(m_bodyLayout);

    auto contentSize = m_bgSprite->getContentSize();
    auto batchNode = getChildOfType<CCSpriteBatchNode>(m_mainLayer,0);
    m_bgSpriteClip = CCClippingNode::create(m_bgSprite);
    m_bgSpriteClip->setAlphaThreshold(0.02f);
    m_bgSpriteClip->setPosition(m_bgSprite->getPosition());
    m_bgSpriteClip->setContentSize(contentSize);
    m_bgSpriteClip->setAnchorPoint(m_bgSprite->getAnchorPoint()); // in case inherited dialogs also modifies this (who will do this)
    //m_bgSprite->removeFromParent();
    m_mainLayer->addChild(m_bgSpriteClip);

    m_bgSpriteClip->addChild(Triangles::create(contentSize,45,ccc3(30,23,30)));
    //m_title->limitLabelWidth(contentSize.width - 2.f, 1.f, .1f);
    
    m_title->setPositionY(m_title->getPositionY() - 71);
    m_title->setFntFile("torus-regular.fnt"_spr);

    auto label = CCLabelBMFont::create(content.c_str(), "torus-regular.fnt"_spr);
    label->setPosition(m_title->getPosition()-CCPoint{0,12});
    label->setScale(0.4);
    label->setZOrder(0);
    m_title->setZOrder(-1);
    m_title->setScale(0.6);

    m_title->removeFromParent();
    m_bodyLayout->addChild(m_title);
    m_bodyLayout->addChild(label);

    auto btnLayer = CCLayerRGBA::create();
    btnLayer->setLayout(
        ColumnLayout::create()
        ->setAutoScale(false)
        ->setAxisReverse(true)
        ->setGap(0)
    );
    btnLayer->setContentWidth(contentSize.width);
    btnLayer->setAnchorPoint(ccp(0.5, 0.5));
    btnLayer->setPosition(ccp(contentSize.width/2,55));
    btnLayer->setCascadeOpacityEnabled(true);

    for (auto& btn : buttons) { btnLayer->addChild(btn); }
    btnLayer->updateLayout();
    m_mainLayer->addChild(btnLayer);

    m_bodyLayout->updateLayout();
    label->limitLabelWidth(contentSize.width - 2.f, 0.4f, .1f);


    this->setOpacity(0);
    FMODAudioEngine::sharedEngine()->playEffect("dialog-pop-in.wav"_spr);
    return true;
}

void PopupDialog::show() {
    FLAlertLayer::show();
    m_mainLayer->stopAllActions();
    m_mainLayer->setScale(0.7);
    m_bgSprite->setOpacity(0);

    m_buttonMenu->setVisible(false);

    m_mainLayer->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.75, 1), 0.5));
    m_bgSprite->runAction(CCEaseOutQuint(CCFadeIn::create(0.2)));

}

void PopupDialog::onClose(cocos2d::CCObject*) {
  hide();
}
void PopupDialog::hide() {
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);

    m_mainLayer->runAction(CCEaseOut::create(CCScaleTo::create(0.5, 0.7f),2));
    CCObject* obj;
    CCARRAY_FOREACH(m_mainLayer->getChildren(), obj) {
        static_cast<CCNode*>(obj)->runAction(CCEaseOutQuint(CCFadeOut::create(0.4)));
    }
    getChildOfType<Triangles>(m_bgSpriteClip, 0)->runAction(CCEaseOutQuint(CCFadeOut::create(0.4)));
    
    this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.5), CCRemoveSelf::create()));
    FMODAudioEngine::sharedEngine()->playEffect("dialog-pop-out.wav"_spr);
}

void PopupDialog::keyBackClicked() {
    this->hide();
}