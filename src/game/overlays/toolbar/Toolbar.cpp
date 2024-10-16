#include "Toolbar.hpp"
#include "../../graphics/OsuColor.hpp"
#include "ToolbarButtons.hpp"

using namespace ToolbarConstants;
bool Toolbar::init() {
    if (!Container::init()) return false;
    m_anchor = Anchor::Top;
    auto bgColor = OsuColor::Gray(0.1f*255);
    setContentSizeWithUnit({1,HEIGHT},Unit::Viewport,Unit::UIKit);
    setAnchorPoint({0.5,1});
    setPositionUnit(Unit::OpenGL,Unit::UIKit);
    setAnchor(Anchor::Top);
    setPositionY(-HEIGHT);

    //auto j = processUnit(TOOLTIP_HEIGHT,Unit::UIKit,false);
    auto j = TOOLTIP_HEIGHT;
    gradient = CCLayerGradient2::create({0,0,0,0},{0,0,0,0},{0,1},CCLG2Target::Start);
    gradient->setAnchorPoint({0,1});// keeping it at the bottom
    gradient->ignoreAnchorPointForPosition(false);
    gradient->setPosition({0,-j});
    gradient->setScaleY(-1);
    addChild(gradient);

    auto left = CCLayer::create();
    left->addChild(ToolbarSettingsButton::create());
    left->addChild(ToolbarHomeButton::create());

    auto lay = 
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(-0.5);
    // this will cause issues
    left->setLayout(lay);
    left->setAnchorPoint({0,0});

    auto right = CCLayer::create();
    right->addChild(ToolbarMusicButton::create());
    right->addChild(ToolbarGeodeButton::create());
    right->addChild(ToolbarModDisableButton::create());

    // this will also cause issues
    right->setLayout(
        lay
        ->setAxisAlignment(AxisAlignment::End)
    );
    right->setAnchorPoint({1,0});
#ifdef GEODE_IS_ANDROID
    left->setPositionX(10);
#endif

    addChild(left);
    addChild(right);
    setColor(bgColor);
    setOpacity(255);

    addListener("nodeLayoutUpdate",[this,j,right](NodeEvent*e){
        gradient->setContentSize({CCNode::getContentSize().width,j});
        right->setPositionX(
            CCNode::getContentSize().width
#ifdef GEODE_IS_ANDROID
            -10
#endif
        );
    });

    return true;
}

void Toolbar::show() {
    if (!shown) runAction(CCEaseOutQuint::create(
        CCMoveTo::create(0.5,{0,0})
    ));
    shown = true;
}

void Toolbar::hide() {
    if (shown) runAction(CCEaseOutQuint::create(
        CCMoveTo::create(0.5,{0,-HEIGHT})
    ));
    shown = false;
}