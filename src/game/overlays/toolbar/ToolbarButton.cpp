#include "ToolbarButton.hpp"
#include "ToolbarConstants.hpp"

bool ToolbarButton::init(IconConstructor icon) {
    OsuClickableContainer::init(sfx);
    iconSprite = icon;
    addListener("nodeLayoutUpdate",[this](NodeEvent* e){
        iconSprite->setPosition(getContentSize()/2);
    });
    addChild(iconSprite);

    setContentSizeWithUnit(CCSize(HEIGHT,HEIGHT),Unit::UIKit,Unit::UIKit);
    return true;
}

void onMouseEnter() {}