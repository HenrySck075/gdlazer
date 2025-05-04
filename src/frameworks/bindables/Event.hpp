#pragma once
#include <Geode/cocos/cocoa/CCObject.h>
#include "../../macro.h"
#include "Geode/loader/Log.hpp"
GDF_NS_START
class Event : public cocos2d::CCObject {
    friend class EventTarget;
    friend class Container;
    friend class Game;
public:
    Event() {
        autorelease();
    }
    bool defaultPrevented() const { return m_defaultPrevented; }
    bool setDispatchOrder(bool reversed) {
        m_dispatchOrderReversed = reversed;
    }
    // currently an alias to stopImmediatePropagation
    // but could be used to block cocos2d from processing the input in the future
    void preventDefault() { m_defaultPrevented = true; }
    void stopPropagation();
    void stopImmediatePropagation();

  private:
    bool m_defaultPrevented = false;
    bool m_propagateStopped = false;
    bool m_immediatePropagateStopped = false;
    bool m_dispatchOrderReversed = false;
};
GDF_NS_END
