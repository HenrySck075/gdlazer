#include "DrawableCarouselBeatmap.hpp"

bool DrawableCarouselBeatmap::init(GJGameLevel* level) {
    auto d = CCScale9Sprite::createWithSpriteFrameName("roundborder.png"_spr);
    d->setColor(ccc3(40, 86, 102));
    d->setAnchorPoint(ccp(0,0));
    m_main = CCClippingNode::create(d);
    m_main->setAlphaThreshold(0.03);
    auto shadow = CCScale9Sprite::createWithSpriteFrameName("roundshadow.png"_spr);
    shadow->setColor(ccc3(0,0,0));
    addChild(m_main);
    addChild(shadow);

    OsuClickableContainer::init("select-expand.wav"_spr, [](CCNode*self){});
    addListener("nodeLayoutUpdate", [this,shadow](NodeEvent* event){
        auto s = getRealContentSize();
        m_main->setContentSize(s);
        m_main->getChildByID("dcb_gradient")->setContentSize(s);
        if(auto stencil = m_main->getStencil()) stencil->setContentSize(s);
        colorBg->setPosition(s/2);
        shadow->setPosition(s/2);
        auto e = processUnit(10,Unit::UIKit,false);
        shadow->setContentSize(s+CCSize{e,e});
    });
    m_level = level;
    colorBg->setZOrder(999);
    colorBg->setOpacity(0);
    colorBg->setAnchorPoint(ccp(0.5,0.5));

    auto gradient = CCLayerGradient::create(ccc4(0,0,0,255),ccc4(0,0,0,0),ccp(1, 0));
    gradient->setZOrder(1000);
    gradient->setID("dcb_gradient");

    m_main->addChild(colorBg);
    m_main->addChild(gradient);
    setAnchorPoint(ccp(0,0));
    setContentSizeWithUnit(CCSize(40,h), Unit::Percent, Unit::UIKit);
    setOpacity(255);
    setColor(ccc3(255, 255, 255));
    return true;

    //CCScheduler::get()->scheduleUpdateForTarget();
}