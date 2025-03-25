#pragma once
#include <Geode/cocos/cocoa/CCObject.h>
#include "../../macro.h"
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
    // currently an alias to stopImmediatePropagation
    // but could be used to block cocos2d from processing the input in the future
    void preventDefault() { m_defaultPrevented = true; }
    void stopPropagation() {m_propagateStopped = true;};
    void stopImmediatePropagation() {
        stopPropagation();
        m_immediatePropagateStopped = true;
    };

private:
    bool m_defaultPrevented = false;
    bool m_propagateStopped = false;
    bool m_immediatePropagateStopped = false;
};
GDF_NS_END
