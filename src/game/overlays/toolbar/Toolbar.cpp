#include "Toolbar.hpp"
#include "ToolbarButton.hpp"

#include "ToolbarButtons.hpp"

using namespace ToolbarConstants;
bool Toolbar::init() {
    Container::init();
    m_anchor = Anchor::Top;
    auto bgColor = OsuColor::Gray(0.1f*255);
    this->setContentSizeWithUnit(CCSize(1,HEIGHT),Unit::Viewport,Unit::UIKit);
    this->setAnchorPoint({0.5,1});
    this->setPositionUnit(Unit::OpenGL,Unit::UIKit);
    this->setPositionY(-HEIGHT);
    this->setAnchor(Anchor::Top);

    //auto j = processUnit(TOOLTIP_HEIGHT,Unit::UIKit,false);
    auto j = TOOLTIP_HEIGHT;
    gradient = CCLayerGradient2::create(ccc4(0,0,0,0),ccc4(0,0,0,0),{0,1},CCLG2Target::Start);
    gradient->setAnchorPoint({0,1});// keeping it at the bottom
    gradient->ignoreAnchorPointForPosition(false);
    gradient->setPosition({0,-j});
    gradient->setScaleY(-1);
    addListener("nodeLayoutUpdate",[this,j](NodeEvent*e){
        gradient->setContentSize({CCNode::getContentSize().width,j});
    });
    addChild(gradient);

    auto left = CCLayer::create();
    left->addChild(ToolbarSettingsButton::create());
    left->addChild(ToolbarHomeButton::create());

    // this will cause issues
    left->setLayout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(-0.5));
    left->setAnchorPoint({0,0});

    auto right = CCLayer::create();
    right->addChild(ToolbarSettingsButton::create());
    right->addChild(ToolbarHomeButton::create());

    // this will also cause issues
    right->setLayout(RowLayout::create()->setAutoScale(false)->setAxisAlignment(AxisAlignment::Start)->setGap(-0.5));
    right->setAnchorPoint({0,0});
#ifdef GEODE_IS_ANDROID
    left->setPositionX(10);
#endif

    addChild(left);
    addChild(right);
    setColor(bgColor);
    setOpacity(255);

    return true;
}
void Toolbar::show() {
    this->runAction(CCEaseOutQuart::create(
        CCMoveFromTo::create(transition_time,{0,-HEIGHT},{0,0})
    ));
}
void Toolbar::hide() {
    this->runAction(CCEaseOutQuart::create(
        CCMoveFromTo::create(transition_time/4,{0,0},{0,-HEIGHT})
    ));
}
