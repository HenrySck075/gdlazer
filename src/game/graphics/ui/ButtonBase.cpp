#include "ButtonBase.hpp"

bool ButtonBase::init(ButtonCallback clickCb, CCNode* self) {
    clickCallback = clickCb;

    this->setZOrder(3);
    //this->setCascadeOpacityEnabled(true);

    CCNodeHover::init();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, -4566);
    m_clickListener = this->template addEventListener<MouseFilter>([this,self](MouseType type, CCPoint location) {
        if (type == MouseType::Click) {
            this->clickCallback(self);
        }
    },false);
    return true;
}

/*
bool ButtonBase::ccTouchBegan(CCTouch* t, CCEvent* idc) {
    bool ret = CCTouchDelegate::ccTouchBegan(t, idc);
    if (ret) {
        log::debug("[CCNodeHover]: {}", touchStartInBounds != m_entered);
        touchStartInBounds = holding = m_entered; 
        clickCallback(this);
    }
    return ret;
}

void ButtonBase::ccTouchEnded(CCTouch* t, CCEvent* idc) {
    if (touchStartInBounds!=m_entered) return;
}

*/