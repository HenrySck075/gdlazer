#include "DialogButton.hpp"
#include "../../../utils.hpp"

DialogButton* DialogButton::create(const char* label, ccColor3B color, const char* clickSfx) {
    auto ret = new DialogButton();
    if (ret && ret->init(label, color, clickSfx)) {
        ret->autorelease();
    }
    else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool DialogButton::init(const char* label, ccColor3B color, const char* clickSfx) {
    m_color = color;
    m_sfx = clickSfx;
    this->setZOrder(3);


    auto d = CCScale9Sprite::createWithSpriteFrameName("thisisasquare.png"_spr);
    if (d!=nullptr) {
        d->setID("dialogbutton-background");
        d->setAnchorPoint(ccp(0.5, 0.5));
        d->setContentHeight(height);
        d->setColor(color);
    } else {
        log::debug("what the fuck?");
    }

    auto clipNode = CCClippingNode::create();
    clipNode->setID("dialogbutton-clipnode");
    clipNode->addChild(Triangles::create(d->getContentSize(),4,color));

    auto j = CCLabelBMFont::create(label, "torus-bold.fnt"_spr);
    j->setID("dialogbutton-label");
    j->setAnchorPoint(ccp(0.5, 0.5));
    j->setScale(0.4);
    // unskew yourself

    // Gradient layers
#define gradientSetup(side) \
    auto grad##side = CCLayerGradient::create(ccc4BFromccc4F(ccc4FFromccc3B(color)), ccc4(color.r, color.g, color.b, 0), ccp(1, 0)); \
    grad##side->setContentSize(CCSize(0,height)); \
    grad##side->setID("gradient"#side); \
    grad##side->setOpacity(0)
    
    gradientSetup(Left);
    gradientSetup(Right);
#undef gradientSetup
    gradLeft->setScaleX(-1);

    this->addChild(gradLeft);
    this->addChild(gradRight);
    if (d!=nullptr) this->addChild(d);
    //this->addChild(clipNode);
    this->addChild(j);
    this->setContentSize(CCSize{ 12.f,height });
    this->setAnchorPoint(CCPoint{ 0.5,0.5 });

    this->setCascadeOpacityEnabled(true);

    CCNodeHover::init();

    return true;

}

void DialogButton::setOpacity(GLubyte opacity) {
    CCNodeRGBA::setOpacity(opacity); 
    getChildOfType<CCLayerGradient>(this, 0)->setOpacity(opacity);
    getChildOfType<CCLayerGradient>(this, 1)->setOpacity(opacity);
}

void DialogButton::setContentSize(const CCSize& size) {
    CCNode::setContentSize(size);
    this->getChildByID("dialogbutton-label")->setPosition(size/2);
    auto d = this->getChildByID("dialogbutton-background");
    if (d!=nullptr) {
        d->setContentWidth(size.width * 0.8);
        d->setPosition(size / 2);
    }

    auto gradWidth = size.width * 0.125;
    
    auto gradLeft = static_cast<CCLayerGradient*>(this->getChildByID("gradientLeft"));
    gradLeft->setContentWidth(gradWidth);
    auto gradRight = static_cast<CCLayerGradient*>(this->getChildByID("gradientRight"));
    gradRight->setContentWidth(gradWidth);
    gradRight->setPosition(ccp(size.width-gradWidth, 0));
}

/*
void DialogButton::setContentWidth(float width) {
    this->setContentSize(CCSize{width,this->getContentHeight()});
}
*/
// no
void DialogButton::setContentHeight(float height) {
}

void DialogButton::setParent(CCNode* parent) {
    CCNode::setParent(parent);
    this->setContentWidth(parent->getContentWidth());
}

void DialogButton::onMouseEnter() {
    FMODAudioEngine::sharedEngine()->playEffect("default-hover.wav"_spr);
    this->getChildByID("dialogbutton-background")->runAction(CCEaseOutQuint(CCResizeTo::create(0.1f,m_pParent->getParent()->getContentWidth()*0.9, height)));
    this->getChildByID("gradientLeft")->runAction(CCEaseOutQuint(CCFadeIn::create(0.1f)));
    this->getChildByID("gradientRight")->runAction(CCEaseOutQuint(CCFadeIn::create(0.1f)));
}
void DialogButton::onMouseExit() {
    this->getChildByID("dialogbutton-background")->runAction(CCEaseOutQuint(CCResizeTo::create(0.1f, m_pParent->getParent()->getContentWidth()*0.8,height)));
    this->getChildByID("gradientLeft")->runAction(CCEaseOutQuint(CCFadeOut::create(0.1f)));
    this->getChildByID("gradientRight")->runAction(CCEaseOutQuint(CCFadeOut::create(0.1f)));
}
