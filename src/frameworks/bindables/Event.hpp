#pragma once
#include <Geode/cocos/cocoa/CCObject.h>
#include <string>
#include "../macro.h"
GDL_NS_START
class Event : public cocos2d::CCObject {
    friend class EventTarget;
    friend class Container;
public:
    explicit Event(const std::string& type) 
        : m_type(type) {
            autorelease();
        }
    std::string type() const { return m_type; }
    bool defaultPrevented() const { return m_defaultPrevented; }
    void preventDefault() { m_defaultPrevented = true; }
    void stopPropagation() {m_propagateStopped = true;};
    void stopImmediatePropagation() {
        stopPropagation();
        m_immediatePropagateStopped = true;
    };

private:
    std::string m_type;
    bool m_defaultPrevented = false;
    bool m_propagateStopped = false;
    bool m_immediatePropagateStopped = false;
};
GDL_NS_END