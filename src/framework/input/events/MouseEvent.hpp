#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../../bindables/Event.hpp"
using namespace cocos2d;

enum MouseEventType {
    Enter, Exit, Click, MouseDown, MouseUp, Move, MouseScroll
};

// mouse event 2
class MouseEvent : public NodeEvent {
public:
    CCPoint position = CCPoint(0,0);
    MouseEventType eventType;
    // not implemented yet
    bool leftClick = true;

    MouseEvent(MouseEventType event, CCPoint pos) : NodeEvent("mouseEvent"), position(pos), eventType(event) {
        //m_log = true;
    };
};