#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../../bindables/Event.hpp"
using namespace cocos2d;

GDL_NS_START
enum class MouseEventType {
    MouseUp,
    MouseDown,
    Move,
    Click,
    Exit, 
    Enter,
    Scroll  // Add scroll type
};

// mouse event 2
class MouseEvent : public Event {
public:
    CCPoint m_position = CCPoint(0,0);
    MouseEventType m_eventType;
    bool m_clicked;
    float m_scrollDelta = 0.0f;  // Add scroll delta

    MouseEvent(MouseEventType event, CCPoint pos, bool clicked)
     : m_position(pos), m_eventType(event), m_clicked(clicked) {
        //setDispatchingFlow(DispatchingFlow::Down);
    };
};
GDL_NS_END
