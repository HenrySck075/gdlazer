#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../../bindables/Event.hpp"
using namespace cocos2d;

enum MouseEventType {
  Enter, Exit, Click, MouseDown, MouseUp, Move, MouseScroll
};

// mouse event 2
class MouseEvent : public Event {
public:
  CCPoint m_position = CCPoint(0,0);
  MouseEventType m_eventType;
  // not implemented yet
  bool m_clicked;

  MouseEvent(MouseEventType event, CCPoint pos, bool clicked)
   : Event("mouseEvent"), m_position(pos), m_eventType(event), m_clicked(clicked) {
    //setDispatchingFlow(DispatchingFlow::Down);
  };
};