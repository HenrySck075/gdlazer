#include "OsuClickableContainer.hpp"

bool OsuClickableContainer::init(std::string clickSfx, ButtonCallback clickCb, CCNode* self) {
    this->clickCallback = clickCb;
    this->clickSfx = clickSfx;

    if (self == nullptr) {self = this;}
    this->setZOrder(3);
    //this->setCascadeOpacityEnabled(true);
    addListener("mouseEvent",[this](NodeEvent* e) {
        auto event = static_cast<MouseEvent*>(e);
        auto type = event->eventType;
        if (!(this->m_hoverEnabled && isRunning())) return;
        switch (type) {
            case MouseEventType::Enter:
                //m_entered = true;
                this->onMouseEnter();
            case MouseEventType::Exit:
                //m_entered = false;
                this->onMouseExit();
            case MouseEventType::MouseUp:
                this->onMouseUp();
            case MouseEventType::MouseDown:
                this->onMouseDown();
            case MouseEventType::Click:
                this->onClick();
                FMODAudioEngine::sharedEngine()->playEffect(this->clickSfx);
                this->clickCallback(this);
        }
        return;
    });
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
