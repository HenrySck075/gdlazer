#pragma once
#include "Easings.hpp"

#define $easings_create(cls, ...) \
cls* ret = new cls(); \
if (ret && ret->initWithAction(__VA_ARGS__)) {} \
else {CC_SAFE_RELEASE(ret);} \
return ret

#define $createEasing(easingName, easingFunc) \
class CCEase##easingName : public cocos2d::CCActionEase { \
public: \
  static CCEase##easingName* create(cocos2d::CCActionInterval* action) { \
    $easings_create(CCEase##easingName, action);\
  } \
  void update(float x) override { \
    m_pInner->update(easings::easingFunc(x)); \
  } \
}

#define $createEasingWithRate(easingName, easingFunc) \
class CCEase##easingName : public cocos2d::CCEaseRateAction { \
public: \
  static CCEase##easingName* create(cocos2d::CCActionInterval* action, float fRate) { \
    $easings_create(CCEase##easingName, action, fRate);\
  } \
  void update(float x) override { \
    m_pInner->update(easings::easingFunc(x,m_fRate)); \
  } \
}

#include <Geode/cocos/actions/CCActionEase.h>

namespace easingsActions {
$createEasing(SineIn, sineIn);

$createEasing(SineOut, sineOut);

$createEasing(SineInOut, sineInOut);


$createEasingWithRate(In, inRate);

$createEasingWithRate(Out, outRate);

$createEasingWithRate(InOut, inOutRate);


$createEasing(ExponentialIn, inExpo);

$createEasing(ExponentialOut, outExpo);

$createEasing(ExponentialInOut, inOutExpo);


$createEasing(ElasticIn, inElastic);

$createEasing(ElasticOut, outElastic);

$createEasing(ElasticInOut, inOutElastic);


$createEasing(BounceIn, inBounce);

$createEasing(BounceOut, outBounce);

$createEasing(BounceInOut, inOutBounce);


$createEasing(BackIn, inBack);

$createEasing(BackOut, outBack);

$createEasing(BackInOut, inOutBack);

}


