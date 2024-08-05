#include "Toolbar.hpp"

bool Toolbar::init() {
    auto color = OsuColor();
    auto bgColor = color.Gray(0.1f);
    this->setContentHeight(uiHeightToGL(HEIGHT));
    
    m_anchor = Anchor::Top;

    auto bg = ContainerNodeWrapper::create(CCResizableSprite::createWithSpriteFrameName("square.png"));
    bg->setContentSize(CCSize(100,100),Unit::Percent,Unit::Percent);

    this->addChild(bg);
    return true;
}