#include "OsuClickableContainer.hpp"
#include "../../../utils.hpp"

bool OsuClickableContainer::init(std::string clickSfx, ButtonCallback clickCb) {
    if (!OsuClickableContainer::init(clickSfx)) return false;
    this->clickCallback = clickCb;
    return true;
}
bool OsuClickableContainer::init(std::string clickSfx) {
    if (!Container::init()) return false;
    this->clickSfx = clickSfx;

    this->setZOrder(3);
    //this->setCascadeOpacityEnabled(true);
    
    //CCNodeHover::init();
    return true;
}


/*
bool OsuClickableContainer::ccTouchBegan(CCTouch* t, CCEvent* idc) {
    bool ret = CCTouchDelegate::ccTouchBegan(t, idc);
    if (ret) {
        log::debug("[CCNodeHover]: {}", touchStartInBounds != m_entered);
        touchStartInBounds = holding = m_entered; 
        clickCallback(this);
    }
    return ret;
}

void OsuClickableContainer::ccTouchEnded(CCTouch* t, CCEvent* idc) {
    if (touchStartInBounds!=m_entered) return;
}

*/
