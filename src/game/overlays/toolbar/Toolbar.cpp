#include "Toolbar.hpp"
#include "../../../helpers/CustomActions.hpp"
#include "ToolbarButton.hpp"

#include "ToolbarButtons.hpp"

using namespace ToolbarConstants;
bool Toolbar::init() {
    Container::init();
    m_anchor = Anchor::Top;
    auto bgColor = OsuColor::Gray(0.1f*255);
    this->setContentSizeWithUnit(CCSize(1,HEIGHT),Unit::Viewport,Unit::UIKit);
    this->setAnchorPoint(ccp(0.5,1));
    this->setPositionUnit(Unit::OpenGL,Unit::UIKit);
    this->setPositionY(-HEIGHT);
    this->setAnchor(Anchor::Top);

    auto gradient = CCLayerGradient::create(ccc4(0,0,0,0),ccc4(0,0,0,0),ccp(0.5,0));
    addListener("nodeLayoutUpdate",[gradient,this](NodeEvent*e){
        gradient->setContentSize(CCSize(getContentWidth(),processUnit(HEIGHT,Unit::UIKit,false)));
    });

    auto left = CCLayer::create();
    left->addChild(ToolbarSettingsButton::create());

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
