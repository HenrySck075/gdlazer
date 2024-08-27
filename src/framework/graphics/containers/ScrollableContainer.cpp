#include "ScrollableContainer.hpp"
void ScrollableContainer::onDragStart(MouseDragEvent* event) {
    lastIdlePos = body->getPosition();
    m_dragging = true;
}
void ScrollableContainer::onDrag(MouseDragEvent* event) {
    lastOffset = body->getPosition();
    lastDragPosition = event->current;
    moveToPos(ccp(lastIdlePos.x, lastIdlePos.y - (event->current.y - event->startPos.y)), false);
    timer = 0;
};
void ScrollableContainer::onDragEnd(MouseDragEvent* event) {
    auto pos = body->getPosition();
    auto distY = lastOffset.y - pos.y;
    bool neg = distY < 0;
    /*
    runAction(CCRepeat::create(CCCallFuncL::create([this,&distY,&pos,&neg](){
        moveToPos(CCPoint(lastOffset.x, pos.y+distY), false);
        if (neg) {
            distY++;
            if (distY >= 0) distY = 0;
        } else {
            distY--;
            if (distY <= 0) distY = 0;
        }
    }),std::max(distY,1)));
    */
    m_dragging = false;
}

void ScrollableContainer::moveToPos(CCPoint const& pos, bool animate) {
    if (animate) {
        body->runAction(CCEaseSineOut::create(CCMoveTo::create(0.25,pos)));
    } else {
        body->setPosition(pos);
    }
}