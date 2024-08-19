#include "OsuClickableContainer.hpp"
#include "../../../utils.hpp"

bool OsuClickableContainer::init(std::string clickSfx, ButtonCallback clickCb, CCNode* self) {
    Container::init();
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
                m_entered = true;
                log::debug("[OsuClickableContainer]: Mouse entered");
                this->onMouseEnter();
                break;
            case MouseEventType::Exit:
                m_entered = false;
                log::debug("[OsuClickableContainer]: Mouse exited");
                this->onMouseExit();
                break;
            case MouseEventType::MouseUp:
                if (m_holding) {
                    this->onMouseUp();
                    m_holding = false;
                }
                break;
            case MouseEventType::MouseDown:
                if (m_entered) {
                    this->onMouseDown();
                    m_holding = true;
                }
                break;
            case MouseEventType::Click:
                log::debug("[OsuClickableContainer]: clicked");
                this->onClick();
                FMODAudioEngine::sharedEngine()->playEffect(this->clickSfx);
                this->clickCallback(this);
                event->preventDefault();
        }
        int type2 = (int)type;
        if (type == MouseEventType::MouseUp && m_entered) {
            type2 = MouseEventType::Click;
        }
        else if (type == MouseEventType::Move) {
            auto bound = boundingBoxFromContentSize(this);
            bool containsCursor = bound.containsPoint(event->position);
            if (
                containsCursor
                && 
                !m_entered
            ) type2 = MouseEventType::Enter;
            if (
                !containsCursor
                && 
                m_entered
            ) type2 = MouseEventType::Exit;
        }
        // redispatch without calling child
        if (type2 != (int)type) dispatchEventUnsafe(new MouseEvent((MouseEventType)type2, event->position));
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
