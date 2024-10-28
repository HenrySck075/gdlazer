#include "ScrollableContainer.hpp"
#include <algorithm>
void ScrollableContainer::onDragStart(MouseDragEvent* event) {
  lastIdlePos = body->getPosition();
  //log::debug("[ScrollableContainer]: sp {}", lastIdlePos);
  m_dragging = true;
}
void ScrollableContainer::onDrag(MouseDragEvent* event) {
  lastOffset = body->getPosition();
  if (lastDragEvent == nullptr) lastDragEvent = event;
  auto dist = (event->current.y - event->startPos.y);
  moveToPos(CCPoint(lastIdlePos.x, -dist+lastIdlePos.y), false);
  timer = 0;
};
void ScrollableContainer::onDragEnd(MouseDragEvent* event) {
  auto pos = body->getPosition();
  auto distY = lastOffset.y - pos.y;
  bool neg = distY < 0;
  float dist = 0.5;
  m_dragging = false;
}

void ScrollableContainer::onMouseScroll(MouseEvent* event) {
  log::debug("[ScrollableContainer]: {}",event->position);
  lastIdlePos = body->getPosition();
  // what the fuck why is is passed in as a horizontal value
  moveToPos(CCPoint(lastIdlePos.x, -(event->position.y*3)+lastIdlePos.y), true);
}

void ScrollableContainer::moveToPos(CCPoint const& pos, bool animate) {
  if (animate) {
    body->runAction(CCEaseSineOut::create(CCMoveTo::create(0.25,pos)));
  } else {
    body->setPosition(pos);
  }
}
