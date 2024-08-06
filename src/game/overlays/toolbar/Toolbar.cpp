#include "Toolbar.hpp"

bool Toolbar::init() {
    Container::init();
    m_anchor = Anchor::Top;
    auto color = OsuColor();
    auto bgColor = color.Gray(0.1f);
    this->setContentHeight(uiHeightToGL(HEIGHT));
    this->setAnchorPoint(ccp(0,1));

    auto bg = ContainerNodeWrapper::create(CCScale9Sprite::createWithSpriteFrameName("square.png"_spr));
    bg->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
    bg->setAnchorPoint(ccp(0,0));

    this->addChild(bg);
    return true;
}