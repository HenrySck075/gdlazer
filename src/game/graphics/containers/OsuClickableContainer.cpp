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
    /*
    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, -4566);
    m_clickListener = this->template addEventListener<MouseFilter>([this,self](MouseType type, CCPoint location) {
        if (isReallyVisible() && clickEnabled) {
            if (type == MouseType::MouseDown) {
                this->onMouseDown();
                return true;
            };
            if (type == MouseType::MouseUp) {
                this->onMouseUp();
                return true;
            };
            if (type == MouseType::Click) {
                FMODAudioEngine::sharedEngine()->playEffect(this->clickSfx.c_str());
                this->onClick();
                this->clickCallback(self);
                return true;
            };
        }
        return false;
    },true);
    */
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
