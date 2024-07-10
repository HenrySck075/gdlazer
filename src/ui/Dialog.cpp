#include "Dialog.hpp"

CCDrawNode* roundedRectangle(CCSize size, float radius, ccColor4F color) {
    auto node = CCDrawNode::create();
    node->setContentSize(size);

    auto innerRectSize = size - CCSize(radius * 2, radius * 2);
    auto iTL = CCPoint(radius, radius);
    auto iTR = CCPoint(innerRectSize.width + radius, radius);
    auto iBL = CCPoint(radius, innerRectSize.height + radius);
    auto iBR = CCPoint(innerRectSize.width + radius, innerRectSize.height + radius);


    node->drawRect(iTL, iBR, color,1,color);

    node->drawRect(iTL - CCPoint(radius, 0), iBL, color,1,color);
    node->drawRect(iTL - CCPoint(0, radius), iTR, color,1,color);
    node->drawRect(iTR, iBR + CCPoint(radius, 0), color,1,color);
    node->drawRect(iBL, iBR + CCPoint(0, radius), color,1,color);

    node->drawCircle(iTL, radius, color,1.f,color, 10);
    node->drawCircle(iTR, radius, color,1.f,color, 10);
    node->drawCircle(iBL, radius, color,1.f,color, 10);
    node->drawCircle(iBR, radius, color,1.f,color, 10);
    // draw it like twice because why
    node->drawCircle(iTL, radius, color, 1.f, color, 10);
    node->drawCircle(iTR, radius, color, 1.f, color, 10);
    node->drawCircle(iBL, radius, color, 1.f, color, 10);
    node->drawCircle(iBR, radius, color, 1.f, color, 10);
    return node;
}

bool osuDialog::setup(std::string const& title, std::string const& content) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // convenience function provided by Popup
    // for adding/setting a title to the popup
    this->setTitle(title);
    m_bodyLayout = CCLayer::create();
    m_bodyLayout->setContentSize(m_bgSprite->getContentSize());
    m_bodyLayout->setAnchorPoint(ccp(0.5, 0.5));
    //m_bodyLayout->setPosition(m_bgSprite->getPosition());
    m_bodyLayout->ignoreAnchorPointForPosition(false);
    m_bodyLayout->setLayout(ColumnLayout::create()->setAutoScale(false)->setGap(0)->setAxisReverse(true));
    m_mainLayer->addChild(m_bodyLayout);

    auto contentSize = m_bgSprite->getContentSize();
    auto batchNode = getChildOfType<CCSpriteBatchNode*>(m_mainLayer,0);
    auto sprit = roundedRectangle(contentSize,6,ccColor4F{rgbColor(34,26,33),1.f});
    m_mainLayer->addChild(sprit);
    sprit->setPosition(m_bgSprite->getPosition());
    sprit->setAnchorPoint(CCPoint{ 0.5,0.5 });
    m_bgSpriteClip = CCClippingNode::create(roundedRectangle(contentSize, 6, ccColor4F{ 0.f,0.f,0.f,1.f }));
    m_bgSpriteClip->setPosition(m_bgSprite->getPosition());
    m_bgSpriteClip->setContentSize(contentSize);
    m_bgSpriteClip->setAnchorPoint(m_bgSprite->getAnchorPoint()); // in case inherited dialogs also modifies this (who will do this)
    m_bgSprite->removeFromParent();
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

    m_bodyLayout->addChild(DialogButton::create("my mom called me for dinner", dialog_button_primary));

    m_bodyLayout->updateLayout();
    label->limitLabelWidth(contentSize.width - 2.f, 0.4f, .1f);


    this->setOpacity(0);
    FMODAudioEngine::sharedEngine()->playEffect("dialog-pop-in.wav"_spr);
    return true;
}

void osuDialog::show() {
    FLAlertLayer::show();
    m_mainLayer->stopAllActions();
    m_mainLayer->setScale(0.7);
    m_bgSprite->setOpacity(0);

    m_buttonMenu->setVisible(false);

    m_mainLayer->runAction(CCEaseElasticOut::create(CCScaleTo::create(0.75, 1), 0.5));
    m_bgSprite->runAction(CCEaseOut::create(CCFadeIn::create(0.2), 5));

}

void osuDialog::onClose(cocos2d::CCObject*) {
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);

    m_mainLayer->runAction(CCEaseOut::create(CCScaleTo::create(0.4, 0.7f),2));
    CCObject* obj;
    CCARRAY_FOREACH(m_mainLayer->getChildren(), obj) {
        static_cast<CCNode*>(obj)->runAction(CCEaseOut::create(CCFadeOut::create(0.4), 5));
    }
    this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.5), CCRemoveSelf::create()));
    FMODAudioEngine::sharedEngine()->playEffect("dialog-pop-out.wav"_spr);
}