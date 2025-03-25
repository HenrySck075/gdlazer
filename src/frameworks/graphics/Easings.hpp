#pragma once
#include "../../macro.h"
namespace easings {
  inline float sineIn(float x);
  inline float sineOut(float x);
  inline float sineInOut(float x);

  inline float inRate(float x, int rate);
  inline float outRate(float x, int rate);
  inline float inOutRate(float x, int rate);

  inline float inExpo(float x);
  inline float outExpo(float x);
  inline float inOutExpo(float x);

  inline float inCirc(float x);
  inline float outCirc(float x);
  inline float inOutCirc(float x);

  inline float inBack(float x);
  inline float outBack(float x);
  inline float inOutBack(float x);

  inline float inElastic(float x);
  inline float outElastic(float x);
  inline float inOutElastic(float x);

  inline float inBounce(float x);
  inline float outBounce(float x);
  inline float inOutBounce(float x);
}
GDF_NS_START
namespace graphics {
  enum class Easing {
    None,
    Out,
    In,
    InQuad,
    OutQuad,
    InOutQuad,
    InCubic,
    OutCubic,
    InOutCubic,
    InQuart,
    OutQuart,
    InOutQuart,
    InQuint,
    OutQuint,
    InOutQuint,
    InSine,
    OutSine,
    InOutSine,
    InExpo,
    OutExpo,
    InOutExpo,
    InCirc,
    OutCirc,
    InOutCirc,
    InElastic,
    OutElastic,
    OutElasticHalf,
    OutElasticQuarter,
    InOutElastic,
    InBack,
    OutBack,
    InOutBack,
    InBounce,
    OutBounce,
    InOutBounce,
    OutPow10,
  };
}
GDF_NS_END
