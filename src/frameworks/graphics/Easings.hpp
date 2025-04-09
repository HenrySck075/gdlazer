#pragma once
#include "../../macro.h"
namespace easings {
  float inSine(float x);
  float outSine(float x);
  float inOutSine(float x);

  float inRate(float x, int rate);
  float outRate(float x, int rate);
  float inOutRate(float x, int rate);

  float inExpo(float x);
  float outExpo(float x);
  float inOutExpo(float x);

  float inCirc(float x);
  float outCirc(float x);
  float inOutCirc(float x);

  float inBack(float x);
  float outBack(float x);
  float inOutBack(float x);

  float inElastic(float x);
  float outElastic(float x);
  float inOutElastic(float x);

  float inBounce(float x);
  float outBounce(float x);
  float inOutBounce(float x);
}
GDF_NS_START
enum class Easing {
  None = 0,
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
GDF_NS_END
