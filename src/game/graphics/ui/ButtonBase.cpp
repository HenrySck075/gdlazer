#include "ButtonBase.hpp"

bool ButtonBase::init(ButtonCallback clickCb) {
    clickCallback = clickCb;

    this->setZOrder(3);
    //this->setCascadeOpacityEnabled(true);

    CCNodeHover::init();

    return true;

}


bool ButtonBase::ccTouchBegan(CCTouch* t, CCEvent* idc) {
  bool ret = CCTouchDelegate::ccTouchBegan(t, idc);
  if (ret) {
    touchStartInBounds = holding = m_entered; 
  }
  return ret;
}
void ButtonBase::ccTouchEnded(CCTouch* t, CCEvent* idc) {
  if (touchStartInBounds!=m_entered) return;
  clickCallback(this);
}
