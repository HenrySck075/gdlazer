#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../../bindables/Event.hpp"
using namespace cocos2d;

GDF_NS_START
enum class MouseEventType {
    MouseUp,
    MouseDown,
    Move,
    Click,
    Exit, 
    Enter
};

// mouse event 2
class MouseEvent : public Event {
public:
  CCPoint m_position {0,0};
  MouseEventType m_eventType;
  bool m_clicked;

  MouseEvent(MouseEventType event, CCPoint pos, bool clicked)
    : m_position(pos), m_eventType(event), m_clicked(clicked) {
      //
  };
};
class MouseScrollEvent : public Event {
public:
  CCPoint m_scrollDelta {0,0};
  /// just in case
  CCPoint m_position {0,0};
  bool m_clicked;

  MouseScrollEvent(CCPoint delta, CCPoint pos, bool clicked)
    : m_position(pos), m_scrollDelta(delta), m_clicked(clicked) {
      //
  };
};
GDF_NS_END
