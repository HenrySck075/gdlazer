#pragma once
#include <Geode/cocos/include/cocos2d.h>
#include "../../bindables/Event.hpp"

GDF_NS_START
enum class MouseDragEventType {
    Start, Move, Stop
};
// This DOES NOT inherit from Event
class MouseDragEvent : public Event {
public:
    MouseDragEvent(MouseDragEventType type, const cocos2d::CCPoint& startPos, 
                  const cocos2d::CCPoint& currentPos, const cocos2d::CCPoint& delta)
        : m_type(type)
        , m_startPosition(startPos)
        , m_currentPosition(currentPos)
        , m_delta(delta) {
            // throw it into the pool
        }

    const cocos2d::CCPoint& getStartPosition() const { return m_startPosition; }
    const cocos2d::CCPoint& getCurrentPosition() const { return m_currentPosition; }
    const cocos2d::CCPoint& getDelta() const { return m_delta; }

    MouseDragEventType m_type;
private:
    cocos2d::CCPoint m_startPosition;
    cocos2d::CCPoint m_currentPosition;
    cocos2d::CCPoint m_delta;
};
GDF_NS_END
