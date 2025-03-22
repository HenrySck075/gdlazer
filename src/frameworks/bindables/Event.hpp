#pragma once
#include <Geode/cocos/cocoa/CCObject.h>
#include "../macro.h"
GDL_NS_START
class Event : public cocos2d::CCObject {
    friend class EventTarget;
    friend class Container;
    friend class Game;
public:
    Event() {
        autorelease();
    }
    bool defaultPrevented() const { return m_defaultPrevented; }
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
GDL_NS_END
