#include "PopupDialogButton.hpp"
#include "../../../utils.hpp"
#include "../../graphics/ui/deco/Triangles.hpp"
#include "../../../helpers/CustomActions.hpp"

PopupDialogButton* PopupDialogButton::create(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb) {
    auto ret = new PopupDialogButton();
    if (ret && ret->init(label, color, clickSfx, clickCb)) {
        ret->autorelease();
    }
    else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool PopupDialogButton::init(const char* label, ccColor3B color, const char* clickSfx, ButtonCallback clickCb) {
    m_color = color;
    this->setZOrder(3);

    auto d = CCScale9Sprite::createWithSpriteFrameName("square.png"_spr);
    if (d!=nullptr) {
        d->setID("dialogbutton-background");
        d->setAnchorPoint(ccp(0.5, 0.5));
        d->setContentHeight(height);
        d->setColor(color);
        d->setSkewX(7);
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

    OsuClickableContainer::init(clickSfx, clickCb, this);

    return true;

}

void PopupDialogButton::setOpacity(GLubyte opacity) {
    CCNodeRGBA::setOpacity(opacity); 
    static_cast<CCLayerGradient*>(this->getChildByID("gradientLeft"))->setOpacity(opacity);
    static_cast<CCLayerGradient*>(this->getChildByID("gradientRight"))->setOpacity(opacity);
}

void PopupDialogButton::setContentSize(const CCSize& size) {
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
void PopupDialogButton::setContentWidth(float width) {
    this->setContentSize(CCSize{width,this->getContentHeight()});
}
*/
// no
void PopupDialogButton::setContentHeight(float height) {
}

void PopupDialogButton::setParent(CCNode* parent) {
    CCNode::setParent(parent);
    this->setContentWidth(parent->getContentWidth());
}

void PopupDialogButton::onMouseEnter() {
    if (!getHoverEnabled()) return;
    FMODAudioEngine::sharedEngine()->playEffect("default-hover.wav"_spr);
    this->getChildByID("dialogbutton-background")->runAction(
        CCEaseOutQuint::create(
            CCResizeTo::create(
                0.1f,
                m_pParent->getParent()->getContentWidth()*hover_width, 
                height
            )
        )
    );
    #define gradAct CCEaseOutQuint::create(CCFadeOut::create(0.1f))
    this->getChildByID("gradientLeft")->runAction(gradAct);
    this->getChildByID("gradientRight")->runAction(gradAct);
    #undef gradAct
}
void PopupDialogButton::onMouseExit() {
    if (!getHoverEnabled()) return;
    this->getChildByID("dialogbutton-background")->runAction(
        CCEaseOutQuint::create(
            CCResizeTo::create(
                0.1f, 
                m_pParent->getParent()->getContentWidth()*idle_width,
                height
            )
        )
    );
    #define gradAct CCEaseOutQuint::create(CCFadeOut::create(0.1f))
    this->getChildByID("gradientLeft")->runAction(gradAct);
    this->getChildByID("gradientRight")->runAction(gradAct);
    #undef gradAct
}
void PopupDialogButton::onMouseDown() {
    this->getChildByID("dialogbutton-background")->runAction(
        CCEaseOutQuad::create(
            CCResizeTo::create(
                click_duration*4, 
                m_pParent->getParent()->getContentWidth()*hover_width*0.98f,
                height
            )
        )
    );
}
void PopupDialogButton::onMouseUp() {
    this->getChildByID("dialogbutton-background")->runAction(
        CCEaseIn::create(
            CCResizeTo::create(
                click_duration, 
                m_pParent->getParent()->getContentWidth()*hover_width,
                height
            ),
            1
        )
    );
}
