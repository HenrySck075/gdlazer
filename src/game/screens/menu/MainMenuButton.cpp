#include "MainMenuButton.hpp" 
#include "../../graphics/ui/OsuText.hpp"
#include "ButtonConstants.hpp"
#include "../../../helpers/CustomActions.hpp"

bool MainMenuButton::init(std::string text, std::string sampleClick, CCLabelBMFont* symbol, Color4 color, ButtonCallback clickAction) {
    ButtonBase::init(sampleClick.c_str(), clickAction, this);
    ReactiveNode::init();

    auto the = CCLayer::create();
    the->setID("ui");
    symbol->setScale(0.3);
    the->setLayout(ColumnLayout::create()->setGap(3)->setAutoScale(false)->setAxisReverse(true));
    the->addChild(symbol);
    auto label = OsuText(text.c_str(), FontType::Regular);
    label->setScale(0.6);
    the->addChild(label);
    the->updateLayout();
    auto s9 = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr);
    s9->setID("background");
    s9->setColor(color);
    s9->setSkewX(7);
    s9->setContentSize(CCSize(BUTTON_WIDTH,BUTTON_AREA_HEIGHT));
    this->addChild(s9);
    this->addChild(the);
    return true;
}

void MainMenuButton::setContentSize(const CCSize& s) {
    ReactiveNode::setContentSize(s);
    this->getChildByID("background")->setContentSize(s);
    this->getChildByID("ui")->setContentSize(s);
}

void MainMenuButton::onMouseEnter() {
    this->getChildByID("background")->runAction(CCEaseElasticOut::create(
        CCResizeTo::create(0.5,BUTTON_WIDTH*1.5,BUTTON_AREA_HEIGHT)
    ));
}
void MainMenuButton::onMouseExit() {
    this->getChildByID("background")->runAction(CCEaseElasticOut::create(
        CCResizeTo::create(0.5,BUTTON_WIDTH,BUTTON_AREA_HEIGHT)
    ));
}