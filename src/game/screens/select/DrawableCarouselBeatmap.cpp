#include "DrawableCarouselBeatmap.hpp"

bool DrawableCarouselBeatmap::init(GJGameLevel* level) {
    m_main = CCClippingNode::create(CCScale9Sprite::createWithSpriteFrameName("roundborder.png"));
    addChild(m_main);
    OsuClickableContainer::init("select-expand.wav"_spr, [](CCNode*self){});
    addListener("nodeLayoutUpdate", [this](NodeEvent* e){
        auto s = getRealContentSize();
        m_main->setContentSize(s);
        m_main->getStencil()->setContentSize(s);
    });
    m_level = level;
    addChild(colorBg);
    setAnchorPoint(ccp(0,0));
    setContentSizeWithUnit(CCSize(100,h), Unit::Percent, Unit::UIKit);
    setOpacity(255);
    setColor(ccc3(255, 255, 255));
    return true;

    //CCScheduler::get()->scheduleUpdateForTarget();
}