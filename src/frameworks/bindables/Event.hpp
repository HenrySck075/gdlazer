#pragma once
#include <Geode/cocos/cocoa/CCObject.h>
#include <string>

class Event : public cocos2d::CCObject {
public:
    explicit Event(const std::string& type, bool canPropagate = true) 
        : m_type(type), m_canPropagate(canPropagate) {
            autorelease();
        }
    std::string type() const { return m_type; }
    bool defaultPrevented() const { return m_defaultPrevented; }
    void preventDefault() { m_defaultPrevented = true; }
    bool canPropagate() const { return m_canPropagate; }

private:
    std::string m_type;
    bool m_defaultPrevented = false;
    bool m_canPropagate;
};