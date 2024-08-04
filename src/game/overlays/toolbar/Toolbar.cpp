#include "Toolbar.hpp"

bool Toolbar::init() {
    auto color = OsuColor();
    auto bgColor = color.Gray(0.1f);
    this->setContentHeight(uiHeightToGL(HEIGHT));
    
    toolbarStyle = Style::create(this);
    toolbarStyle->anchor->horizontal->setObject(AnchorHorizontal::Center);
    toolbarStyle->anchor->vertical->setObject(AnchorVertical::Top);
    toolbarStyle->offset ->setObject(ccp(0,0));
    toolbarStyle->size->setObject(CCSize(1,HEIGHT));
    toolbarStyle->size_unit->horizontal->setObject(Unit::Viewport);
    toolbarStyle->size_unit->vertical->setObject(Unit::UIKit);

    auto bg = CCResizableSprite::createWithSpriteFrameName("square.png");
    auto bgStyle = Style::create(bg);
    bgStyle->size->setObject(CCSize(100,100));
    bgStyle->size_unit->horizontal->setObject(Unit::Percent);
    bgStyle->size_unit->vertical->setObject(Unit::Percent);

    this->addChild(bg);
    return true;
}