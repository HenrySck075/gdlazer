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
  // screen generally are not something that has opacity but if you have any opinions feel free to tell me
  // the only screens that does that are intro screens
  setColor({0,0,0,0});
  return true;
};
