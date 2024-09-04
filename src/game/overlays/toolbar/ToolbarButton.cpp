#include "ToolbarButton.hpp"
#include "ToolbarConstants.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../graphics/ui/OsuText.hpp"

using namespace ToolbarConstants;

bool ToolbarButton::init(IconConstructor icon, std::string label, std::string sub) {
    OsuClickableContainer::init(sfx);
    iconSprite = icon;
    addListener("nodeLayoutUpdate",[this](NodeEvent* e){
        iconSprite->setPosition(getContentSize()/2);
    });
    addChild(iconSprite);

    bg = Container::create();
    bg->setColor(OsuColor::Gray(80));
    bg->setPadding(Vector4(3));
    bg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
    bg->setOpacity(0);
    flashBg = Container::create();
    flashBg->setPadding(Vector4(3));
    flashBg->setContentSizeWithUnit(CCSize(100,100), Unit::Percent, Unit::Percent);
    flashBg->setOpacity(0);
    addChild(bg);
    addChild(flashBg);

    tooltipContainer = CCLayer::create();
    tooltipContainer->setAnchorPoint(ccp(0,1));
    text = OsuText(label.c_str(), FontType::Bold);
    subtext = OsuText(sub.c_str(), FontType::Regular);
    tooltipContainer->addChild(text);
    tooltipContainer->addChild(subtext);
    tooltipContainer->setLayout(ColumnLayout::create()->setGap(2)->setAutoScale(false));
    addChild(tooltipContainer);

    setContentSizeWithUnit(CCSize(HEIGHT,HEIGHT),Unit::UIKit,Unit::UIKit);
    return true;
}

void ToolbarButton::onMouseEnter() {
    bg->runAction(CCFadeTo::create(0.2, 180));
}
void ToolbarButton::onMouseExit() {
    bg->runAction(CCFadeTo::create(0.2, 0));
}
void ToolbarButton::onClick(MouseEvent* e) {
    flashBg->runAction(CCSequence::createWithTwoActions(
        CCFadeTo::create(0.05,100),
        CCFadeTo::create(0.8,0)
    ));
};
