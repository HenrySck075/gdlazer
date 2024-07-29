#include "MainMenuButton.hpp" 
#include "../../graphics/ui/OsuText.hpp"
#include "ButtonConstants.hpp"
#include "../../../helpers/CustomActions.hpp"

bool MainMenuButton::init(std::string text, std::string sampleClick, CCLabelBMFont* symbol, Color4 color, ButtonCallback clickAct) {
    auto m = CCSize(BUTTON_WIDTH,BUTTON_AREA_HEIGHT);
    ButtonBase::init(sampleClick, clickAct, this);
    ButtonBase::setContentSize(m);
    //this->addListener(reactive_listener(updateReactive));

    auto the = CCLayer::create();
    the->setID("ui");
    symbol->setScale(0.3);
    the->setLayout(ColumnLayout::create()->setGap(3)->setAutoScale(false)->setAxisReverse(true));
    the->addChild(symbol);
    the->setAnchorPoint(ccp(0.5,0.5));
    the->setPosition(m/2);
    auto label = OsuText(text.c_str(), FontType::Regular);
    label->setScale(0.4);
    the->addChild(label);
    the->updateLayout();
    auto s9 = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr);
    s9->setID("background");
    s9->setColor(color);
    s9->setSkewX(7);
    s9->setContentSize(m);
    s9->setAnchorPoint(ccp(0.5,0.5));
    s9->setPosition(m/2);
    this->addChild(s9);
    this->addChild(the);
    
    setCascadeOpacityEnabled(true);
    this->setAnchorPoint(ccp(0.5,0.5));
    return true;
}


void MainMenuButton::setContentSize(const CCSize& s) {
    ButtonBase::setContentSize(s);
    this->getChildByID("background")->setContentSize(s);
    this->getChildByID("background")->setPosition(s/2);
    this->getChildByID("ui")->setPosition(s/2);
    m_pParent->updateLayout();
}

void MainMenuButton::onMouseEnter() {
    this->runAction(CCEaseElasticOut::create(
        CCResizeTo::create(0.5,BUTTON_WIDTH*1.5,BUTTON_AREA_HEIGHT)
    ));
    FMODAudioEngine::sharedEngine()->playEffect("button-hover.wav"_spr);
}
void MainMenuButton::onMouseExit() {
    this->runAction(CCEaseElasticOut::create(
        CCResizeTo::create(0.5,BUTTON_WIDTH,BUTTON_AREA_HEIGHT)
    ));
}