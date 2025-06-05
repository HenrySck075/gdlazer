#pragma once
#include "Easings.hpp"
#include "../transforms/DefaultEasingFunc.hpp"

#include <Geode/cocos/actions/CCActionEase.h>

GDF_NS_START
class ActionEase : public cocos2d::CCActionEase {
protected:
  DefaultEasingFunction m_easingFunc;
  bool initWithEasing(cocos2d::CCActionInterval* action, Easing easing) {
    if (!initWithAction(action)) return false;
    m_easingFunc = DefaultEasingFunction(easing);
    return true;
  };
public:
  static ActionEase* create(cocos2d::CCActionInterval* action, Easing easing) {
    $createClass(ActionEase, initWithEasing, action, easing);
  }
  void update(float x) override {
    m_pInner->update(m_easingFunc.applyEasing(x));
  }
};
GDF_NS_END