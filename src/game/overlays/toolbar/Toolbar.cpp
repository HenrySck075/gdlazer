#include "Toolbar.hpp"
#include "../../../helpers/CustomActions.hpp"

bool Toolbar::init() {
    Container::init();
    addChild(colorBg);
    m_anchor = Anchor::Top;
    auto bgColor = OsuColor::Gray(0.1f*255);
    this->setSizeUnit(Unit::OpenGL,Unit::UIKit);
    this->setContentHeight(HEIGHT);
    this->setAnchorPoint(ccp(0.5,1));
    this->setPositionUnit(Unit::OpenGL,Unit::UIKit);
    this->setPositionY(-HEIGHT);
    this->setAnchor(Anchor::Top);

    setColor(bgColor);
    setOpacity(255);

    return true;
}
void Toolbar::show() {
    this->runAction(CCEaseOutQuart::create(
        CCMoveFromTo::create(transition_time,ccp(0,-HEIGHT),ccp(0,0))
    ));
}
void Toolbar::hide() {
    this->runAction(CCEaseOutQuart::create(
        CCMoveFromTo::create(transition_time/4,ccp(0,0),ccp(0,-HEIGHT))
    ));
}