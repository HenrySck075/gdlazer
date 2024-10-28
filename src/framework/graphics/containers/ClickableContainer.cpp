#include "ClickableContainer.hpp"
#include "../../../utils.hpp"

bool ClickableContainer::init(std::string clickSfx, ButtonCallback clickCb) {
  if (!ClickableContainer::init(clickSfx)) return false;
  this->clickCallback = clickCb;
  return true;
}
bool ClickableContainer::init(std::string clickSfx) {
  if (!Container::init()) return false;
  this->clickSfx = clickSfx;

  this->setZOrder(3);
  //this->setCascadeOpacityEnabled(true);
  
  //CCNodeHover::init();
  return true;
}


/*
bool ClickableContainer::ccTouchBegan(CCTouch* t, CCEvent* idc) {
  bool ret = CCTouchDelegate::ccTouchBegan(t, idc);
  if (ret) {
    log::debug("[CCNodeHover]: {}", touchStartInBounds != m_entered);
    touchStartInBounds = holding = m_entered; 
    clickCallback(this);
  }
  return ret;
}

void ClickableContainer::ccTouchEnded(CCTouch* t, CCEvent* idc) {
  if (touchStartInBounds!=m_entered) return;
}

*/
