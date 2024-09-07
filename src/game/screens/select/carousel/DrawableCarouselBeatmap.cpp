#include "DrawableCarouselBeatmap.hpp"
#include "../../../graphics/OsuColor.hpp"
#include "../../../graphics/ui/OsuText.hpp"
#include "henrysck075.imagecache/include/ImageCache.hpp"
#include <fmt/format.h>

bool DrawableCarouselBeatmap::init(GJGameLevel* level) {
    auto d = CCScale9Sprite::createWithSpriteFrameName("roundborder.png"_spr);
    d->setColor(ccc3(40, 86, 102));
    d->setAnchorPoint(ccp(0,0));
    m_main = CCClippingNode::create(d);
    m_main->setAlphaThreshold(0.03);
    m_shadow = CCScale9Sprite::createWithSpriteFrameName("roundshadow.png"_spr);
    m_shadow->setColor(ccc3(0,0,0));
    addChild(m_main);
    addChild(m_shadow);

    OsuClickableContainer::init("select-expand.wav"_spr, [](CCNode*self){});
    m_level = level;
    colorBg = CCLayerColor::create();
    colorBg->setZOrder(999);
    colorBg->setAnchorPoint(ccp(0.5,0.5));

    auto gradient = CCLayerGradient::create(ccc4(0,0,0,255),ccc4(0,0,0,0),ccp(1, 0));
    gradient->setZOrder(1000);
    gradient->setID("dcb_gradient");

    m_main->addChild(colorBg);
    m_main->addChild(gradient);
    setAnchorPoint(ccp(0,0));
    setContentSizeWithUnit(CCSize(100,h), Unit::Percent, Unit::UIKit);
    setOpacity(0);
    setColor(OsuColor::Blue);

    auto levelName = OsuText(level->m_levelName.c_str(), FontType::Regular);
    levelName->setAnchorPoint(ccp(0,1));
    levelName->setScale(0.4);
    levelName->setZOrder(50);
    addChild(levelName);
    levelName->setPosition(ccp(0,getContentSize().height));

    m_thumbnail = CCResizableSprite::create();
    m_thumbnail->preserveRatio(true);
    m_thumbnail->setAnchorPoint(CCPoint(0,0));

    if (level->m_isUploaded) {
        // stealing from level thumbnails
        ImageCache::instance()->download(
            fmt::format("https://raw.githubusercontent.com/cdc-sys/level-thumbnails/main/thumbs/{}.png",(int)level->m_levelID), 
            {}, 
            fmt::format("thumb-{}",(int)level->m_levelID), 
            [this](CCImage* img, std::string k) {
                auto texture = new CCTexture2D();
                texture->initWithImage(img);
                auto s = texture->getContentSize();
                m_thumbnail->getSprite()->setTexture(texture);
                m_thumbnail->getSprite()->setTextureRect({0,0,s.width,s.height});
                m_thumbnail->refreshScaling();
            }
        );
    }

    addListener("nodeLayoutUpdate", [this,levelName](NodeEvent* event){
        auto s = getContentSize();
        m_thumbnail->setContentSize(s);
        levelName->setPosition(ccp(0,s.height));
        m_main->setContentSize(s);
        m_main->getChildByID("dcb_gradient")->setContentSize(s);
        if(auto stencil = m_main->getStencil()) stencil->setContentSize(s);
        //colorBg->setPosition(s/2);
        m_shadow->setPosition(s/2);
        auto e = processUnit(9,Unit::UIKit,false);
        m_shadow->setContentSize(s+CCSize{e,e});
    });
    return true;

    //CCScheduler::get()->scheduleUpdateForTarget();
}

void DrawableCarouselBeatmap::onClick()  {
    select();
}
void DrawableCarouselBeatmap::select() {
    m_shadow->setColor(ccc3(130, 204, 255));
    m_shadow->setOpacity(150);
    auto e = new CarouselItemSelect();
    dispatchEvent(e);
}
void DrawableCarouselBeatmap::deselect() {
    m_shadow->setColor(ccc3(0,0,0));
    m_shadow->setOpacity(255);
}


void DrawableCarouselBeatmap::onMouseEnter() {
    colorBg->stopAllActions();
    colorBg->runAction(CCFadeTo::create(0.1,25));
}

void DrawableCarouselBeatmap::onMouseExit() {
    colorBg->stopAllActions();
    colorBg->runAction(CCFadeTo::create(0.25,0));
}
