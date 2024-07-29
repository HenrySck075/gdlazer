#include "ButtonBase.hpp"

bool ButtonBase::init(std::string clickSfx, ButtonCallback clickCb, CCNode* self) {
    this->clickCallback = clickCb;
    this->clickSfx = clickSfx;
    log::debug("[ButtonBase]: {}", clickSfx);

    this->setZOrder(3);
    //this->setCascadeOpacityEnabled(true);

    CCNodeHover::init();
    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, -4566);
    m_clickListener = this->template addEventListener<MouseFilter>([this,self](MouseType type, CCPoint location) {
        if (type == MouseType::Click) {
            log::debug("[ButtonBase]: sfx: {}", this->clickSfx);
            
            FMODAudioEngine::sharedEngine()->playEffect(this->clickSfx.c_str());
            this->clickCallback(self);
            return true;
        };
        return false;
    },true);
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