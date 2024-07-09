#include "DialogButton.hpp"
#include "../utils.hpp"

DialogButton* DialogButton::create(std::string& label, ccColor3B color) {
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

bool DialogButton::init(std::string& label, ccColor3B color) {
    m_color = color;
    this->setContentSize(CCSize{ 6.f,height });
    auto d = CCDrawNode::create();
    d->setID("dialogbutton-background");

    auto clipNode = CCClippingNode::create();
    clipNode->setID("dialogbutton-clipnode");
    clipNode->addChild(Triangles::create(d->getContentSize(),4,color));

    auto j = CCLabelBMFont::create(label.c_str(), "torus-bold.fnt"_spr);
    j->setID("dialogbutton-label");
    j->setAnchorPoint(ccp(0.5, 0.5));

    this->addChild(clipNode);
    this->addChild(d);
    this->redraw();
    this->setAnchorPoint(CCPoint{ 0.5,0.5 });

    return true;
}

void DialogButton::redraw() {
    auto d = static_cast<CCDrawNode*>(this->getChildByID("dialogbutton-background"));
    d->clear();
    d->setContentSize(this->getContentSize());
    auto color = m_color;
    auto color4F = ccColor4F{ rgbColor(color.r,color.g,color.b),1.f };
    CCPoint leftedgePolygon[3] = { CCPoint{0.f,0.f}, CCPoint{3.f,0.f}, CCPoint{3.f,height} };
    d->drawPolygon(leftedgePolygon, 3, color4F, 0, ccColor4F{ 0,0,0,0.f });
    auto width = this->getContentWidth();
    CCPoint rightedgePolygon[3] = { CCPoint{width,0.f}, CCPoint{width-3,0.f}, CCPoint{width-3,height} };
    d->drawPolygon(rightedgePolygon, 3, color4F, 0, ccColor4F{ 0,0,0,0.f });

    d->drawRect(ccp(3, height), ccp(width - 3, 0), color4F, 0, color4F);

    auto clipNode = static_cast<CCClippingNode*>(this->getChildByID("dialogbutton-clipnode"));
    getChildOfType<Triangles>(clipNode, 0)->setContentSize(this->getContentSize());

    getChildOfType<CCLabelBMFont>(this, 0)->setPosition(this->getContentSize() / 2);
}

void DialogButton::setContentSize(const CCSize& size) {
    CCNode::setContentSize(size);
    this->redraw();
}

void DialogButton::setParent(CCNode* parent) {
    CCNode::setParent(parent);
    this->setContentWidth(parent->getContentWidth() * 0.8);
}

void DialogButton::onMouseAccessState(MouseMoveType type, CCPoint location) {
	if (type == MouseMoveType::Enter) {
		this->onMouseEnter();
	}
	else {
		this->onMouseExit();
}
};

void DialogButton::onMouseEnter() {
    FMODAudioEngine::sharedEngine()->playEffect("button-hover.wav");
}
void DialogButton::onMouseExit() {
}