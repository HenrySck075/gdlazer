#pragma once
#include "../../bindables/Event.hpp"
#include <Geode/cocos/include/cocos2d.h>

GDL_NS_START
class MouseDragEvent : public Event {
public:
    MouseDragEvent(const std::string& type, const cocos2d::CCPoint& startPos, 
                  const cocos2d::CCPoint& currentPos, const cocos2d::CCPoint& delta)
        : Event(type)
        , m_startPosition(startPos)
        , m_currentPosition(currentPos)
        , m_delta(delta) {}

    const cocos2d::CCPoint& getStartPosition() const { return m_startPosition; }
    const cocos2d::CCPoint& getCurrentPosition() const { return m_currentPosition; }
    const cocos2d::CCPoint& getDelta() const { return m_delta; }

private:
    cocos2d::CCPoint m_startPosition;
    cocos2d::CCPoint m_currentPosition;
    cocos2d::CCPoint m_delta;
};
GDL_NS_END