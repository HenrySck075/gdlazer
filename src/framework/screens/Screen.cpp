#include "Screen.hpp"
#include "../input/events/KeyEvent.hpp"
#include "Geode/cocos/keypad_dispatcher/CCKeypadDispatcher.h"

bool Screen::init() {
    if (!Container::init()) return false;
    setContentSizeWithUnit(CCSize(100, 100), Unit::Percent, Unit::Percent);
    addListener("keypadEvent", [this](NodeEvent* e){
        if (static_cast<KeypadEvent*>(e)->key == cocos2d::kTypeBackClicked) {
            backButton();
        }
    });
    return true;
};
