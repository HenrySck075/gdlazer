#include "ToolbarButton.hpp"
#include "ToolbarConstants.hpp"

using namespace ToolbarConstants;

bool ToolbarButton::init(IconConstructor icon, std::string text, std::string sub) {
    OsuClickableContainer::init(sfx);
    iconSprite = icon;
    addListener("nodeLayoutUpdate",[this](NodeEvent* e){
        iconSprite->setPosition(getContentSize()/2);
    });
    addChild(iconSprite);

    bg = Container::create();
    bg->setPadding(Vector4(3));
    bg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
    bg->setOpacity(0);
    hbg = Container::create();
    hbg->setPadding(Vector4(3));
    hbg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
    hbg->setOpacity(0);
    addChild(bg);
    addChild(hbg);

    setContentSizeWithUnit(CCSize(HEIGHT,HEIGHT),Unit::UIKit,Unit::UIKit);
    return true;
}

void ToolbarButton::onMouseEnter() {
    bg->runAction(CCFadeTo::create(0.25, 150));
}
void ToolbarButton::onMouseExit() {
    bg->runAction(CCFadeTo::create(0.25, 0));
}
void ToolbarButton::onMouseDown(MouseEvent* e) {
    hbg->runAction(CCFadeTo::create(0.25, 90));
};
void ToolbarButton::onMouseUp(MouseEvent* e) {
    hbg->runAction(CCFadeTo::create(0.25, 0));
};
