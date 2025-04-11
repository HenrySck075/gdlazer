#pragma once
#include "../../bindables/Event.hpp"
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"

GDF_NS_START
struct KeyEvent : public Event {
  const cocos2d::enumKeyCodes m_key;
  const bool m_pressed;
  struct {
    bool ctrl;
    bool shift;
    bool alt;
    bool cmd;
  } m_modifiers;
  KeyEvent(
    const cocos2d::enumKeyCodes key,
    bool pressed,
    decltype(m_modifiers) modifiers
  )
    : m_key(key), m_pressed(pressed), m_modifiers(modifiers) {}
};
GDF_NS_END
