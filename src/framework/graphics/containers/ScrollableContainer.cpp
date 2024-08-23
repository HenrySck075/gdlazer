#include "ScrollableContainer.hpp"

void ScrollableContainer::onDrag(MouseDragEvent* event) {
    stopAction(lastOffsetResetAction);
    lastOffset = body->getRealPosition();
    moveToPos(ccp(lastIdlePos.x, lastIdlePos.y + (event->current.y - event->startPos.y)), false);
    runAction(lastOffsetResetAction);
};
void ScrollableContainer::onDragEnd(MouseDragEvent* event) {
    auto pos = body->getRealPosition();
    auto distY = lastOffset.y - pos.y;
    bool neg = distY < 0;
    runAction(CCRepeat::create(CCCallFuncL::create([this,&distY,&pos,&neg](){
        moveToPos(CCPoint(lastOffset.x, pos.y+distY), false);
        if (neg) {
            distY++;
            if (distY >= 0) distY = 0;
        } else {
            distY--;
            if (distY <= 0) distY = 0;
        }
    }),distY));
}

void ScrollableContainer::moveToPos(CCPoint const& pos, bool animate) {
    if (animate) {
        body->runAction(CCEaseSineOut::create(CCMoveTo::create(0.25,pos)));
    } else {
        body->setPosition(pos);
    }
}