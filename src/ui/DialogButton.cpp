#include "DialogButton.hpp"
#include "../utils.hpp"

DialogButton* DialogButton::create(const char* label, ccColor3B color) {
    auto ret = new DialogButton();
    if (ret && ret->init(label, color)) {
        ret->autorelease();
    }
    else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool DialogButton::init(const char* label, ccColor3B color) {
    m_color = color;
    this->setZOrder(3);
    auto d = CCDrawNode::create();
    d->setID("dialogbutton-background");
    this->setSkewX(30);
    //d->setContentSize(CCSize{ 6.f,height });

    auto clipNode = CCClippingNode::create();
    clipNode->setID("dialogbutton-clipnode");
    clipNode->addChild(Triangles::create(d->getContentSize(),4,color));

    auto j = CCLabelBMFont::create(label, "torus-bold.fnt"_spr);
    j->setID("dialogbutton-label");
    j->setAnchorPoint(ccp(0.5, 0.5));
    j->setScale(0.5);
    // unskew yourself
    j->setSkewX(-30);

    this->addChild(d);
    //this->addChild(clipNode);
    this->addChild(j);
    this->setContentSize(CCSize{ 12.f,height });
    this->setAnchorPoint(CCPoint{ 0.5,0.5 });


    m_listener = this->template addEventListener<MouseEnterExitFilter>([this](MouseMoveType type, CCPoint location) {
        if (type == MouseMoveType::Enter) {
            this->onMouseEnter();
            return;
        }
        if (type == MouseMoveType::Exit) {
            this->onMouseExit();
        }
    }, false);

    return true;

}

void DialogButton::redraw() {
    auto d = static_cast<CCDrawNode*>(this->getChildByID("dialogbutton-background"));
    d->clear();
    
    d->setContentSize(this->getContentSize());
    auto color = m_color;
    auto color4F = ccColor4F{ rgbColor(color.r,color.g,color.b),1.f };
    auto width = this->getContentWidth();
    /*
    CCPoint leftedgePolygon[3] = { CCPoint{0.f,0.f}, CCPoint{6,0.f}, CCPoint{6.f,height} };
    d->drawPolygon(leftedgePolygon, 3, color4F, 0, ccColor4F{ 0,0,0,0.f });
    CCPoint rightedgePolygon[3] = { CCPoint{width-6,1}, CCPoint{width,height}, CCPoint{width-6,height} };
    d->drawPolygon(rightedgePolygon, 3, color4F, 0, ccColor4F{ 0,0,0,0.f });
    */
    d->drawRect(ccp(0, height), ccp(width, 0), color4F, 0, color4F);
    /*
    auto clipNode = static_cast<CCClippingNode*>(this->getChildByID("dialogbutton-clipnode"));
    getChildOfType<Triangles>(clipNode, 0)->setContentSize(this->getContentSize());
    */
    getChildOfType<CCLabelBMFont>(this, 0)->setPosition(this->getContentSize() / 2);
}

void DialogButton::setContentSize(const CCSize& size) {
    CCNode::setContentSize(size);
    this->redraw();
}
/*
void DialogButton::setContentWidth(float width) {
    this->setContentSize(CCSize{width,this->getContentHeight()});
}
void DialogButton::setContentHeight(float height) {
    this->setContentSize(CCSize{ this->getContentWidth(), height });
}
*/

void DialogButton::setParent(CCNode* parent) {
    CCNode::setParent(parent);
    this->setContentWidth(parent->getContentWidth() * 0.8);
}

void DialogButton::onMouseEnter() {
    FMODAudioEngine::sharedEngine()->playEffect("button-hover.wav");
    this->runAction(CCEaseOutQuint(CCResizeTo::create(0.1f,m_pParent->getParent()->getContentWidth()*0.9, this->getContentHeight())));
}
void DialogButton::onMouseExit() {
    this->runAction(CCEaseOutQuint(CCResizeTo::create(0.1f, m_pParent->getParent()->getContentWidth()*0.8,this->getContentHeight())));
}