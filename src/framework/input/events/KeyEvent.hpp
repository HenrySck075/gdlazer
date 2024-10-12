#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../../bindables/Event.hpp"
#include "Geode/cocos/keypad_dispatcher/CCKeypadDispatcher.h"
using namespace cocos2d;

struct KeyInfo {
    enumKeyCodes key;
    // also true for command key because idk i havent used a mac
    bool ctrl; 
    bool alt;
    bool shift;

    // false if this is a release event
    bool pressed;
};

class KeyboardEvent final : public NodeEvent {
public:
    KeyInfo key;
    KeyboardEvent(KeyInfo k) : NodeEvent("keyboardEvent"), key(k) {};
};

class KeypadEvent final : public NodeEvent {
public:
    ccKeypadMSGType key;
    KeypadEvent(ccKeypadMSGType k) : NodeEvent("keyadEvent"), key(k) {};
};
