#include "Rectangle.hpp"

CCRectangle* CCRectangle::create(ccColor4F color) {
    auto ret = new CCRectangle();
    if (ret && ret->init(color)) {
        ret->autorelease();
    }
    else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool CCRectangle::init(ccColor4F color) {
    m_color = color;
    auto drawnode = CCDrawNode::create();
    this->addChild(drawnode);
    return true;
}

void CCRectangle::setContentSize(const CCSize& size) {
    CCNode::setContentSize(size);
    this->redraw();
}

void CCRectangle::redraw() {
    auto drawnode = getChildOfType<CCDrawNode>(this, 0);
    drawnode->clear();
    drawnode->drawRect(ccp(0, 0), CCPoint(this->getContentSize()), m_color, 0, m_color);
}

void CCRectangle::setOpacity(GLubyte opacity) {
    CCNodeRGBA::setOpacity(opacity);
    m_color.a = opacity / 100;
    this->redraw();
}