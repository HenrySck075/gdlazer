#pragma once
#include "../../bindables/Event.hpp"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"

GDF_NS_START
struct KeyEvent : public Event {
  const cocos2d::enumKeyCodes m_key;
  const bool m_pressed;
  KeyEvent(
    const cocos2d::enumKeyCodes key,
    bool pressed
  )
    : m_key(key), m_pressed(pressed) {}
};
GDF_NS_END
