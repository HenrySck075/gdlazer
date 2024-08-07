#include "Toolbar.hpp"
#include "../../../helpers/CustomActions.hpp"

bool Toolbar::init() {
    Container::init();
    m_anchor = Anchor::Top;
    auto color = OsuColor();
    auto bgColor = color.Gray(0.1f);
    this->setSizeUnit(Unit::OpenGL,Unit::UIKit);
    this->setContentHeight(HEIGHT);
    this->setAnchorPoint(ccp(0.5,1));
    this->setPositionUnit(Unit::OpenGL,Unit::UIKit);
    this->setPositionY(-HEIGHT);

    auto bg = ContainerNodeWrapper::create(
        CCScale9Sprite::createWithSpriteFrameName("square.png"_spr)
    );
    bg->setContentSizeWithUnit(CCSize(100,100),Unit::Percent,Unit::Percent);
    bg->setAnchorPoint(ccp(0,0));
    static_cast<CCScale9Sprite*>(bg->getWrappingNode())->setColor(bgColor);

    this->addChild(bg);
    return true;
}
void Toolbar::show() {
    this->runAction(CCEaseOutQuint::create(
        CCMoveFromTo::create(transition_time,ccp(0,-HEIGHT),ccp(0,0))
    ));
}
void Toolbar::hide() {
    this->runAction(CCEaseOutQuint::create(
        CCMoveFromTo::create(transition_time/4,ccp(0,0),ccp(0,-HEIGHT))
    ));
}