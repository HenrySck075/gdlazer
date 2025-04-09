
#include "Easings.hpp"
#include <cmath>

namespace easings {
  float inSine(float x) {
    return 1 - cosf((x * M_PI) / 2);
  }
  float outSine(float x) {
    return sinf((x * M_PI) / 2);
  }
  float inOutSine(float x) {
    return -(cosf(M_PI * x) - 1) / 2;
  }

  float inRate(float x, int rate) {
    return powf(x, rate);
  }
  float outRate(float x, int rate) {
    return 1 - powf(1 - x, rate);
  }
  float inOutRate(float x, int rate) {
    return x < 0.5
      ? powf(2, rate - 1) * inRate(x, rate)
      : 1 - powf(-2 * x + 2, rate) / 2;
  }

  float inExpo(float x) {
    return x == 0 ? 0 : powf(2, 10 * x - 10);
  }
  float outExpo(float x) {
    return x == 1 ? 1 : 1 - powf(2, -10 * x);
  }
  float inOutExpo(float x) {
    return x == 0
      ? 0
      : x == 1
      ? 1
      : x < 0.5 ? powf(2, 20 * x - 10) / 2
      : (2 - powf(2, -20 * x + 10)) / 2;
  }

  float inCirc(float x) {
    return 1 - sqrtf(1 - powf(x, 2));
  }
  float outCirc(float x) {
    return sqrtf(1 - powf(x - 1, 2));
  }
  float inOutCirc(float x) {
    return x < 0.5
      ? inCirc(2 * x) / 2 
      : outCirc(-2 * x + 3) + 1;
  }

  static const float c1 = 1.70158f;
  static const float c3 = c1 + 1;
  float inBack(float x) {
    return c3 * x * x * x - c1 * x * x;
  }
  float outBack(float x) {
    return 1 + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
  }
  float inOutBack(float x) {
    static const float c2 = c1 * 1.525;

    return x < 0.5
      ? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
      : (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
  }

  static const float c4 = (2 * M_PI) / 3;
  float inElastic(float x) {
    return x == 0
      ? 0
      : x == 1
      ? 1
      : -powf(2, 10 * x - 10) * sinf((x * 10 - 10.75) * c4);
  }
  float outElastic(float x) {
    return x == 0
      ? 0
      : x == 1
      ? 1
      : powf(2, -10 * x) * sinf((x * 10 - 0.75) * c4) + 1;
  }
  float inOutElastic(float x) {
    static const float c5 = (2 * M_PI) / 4.5;

    return x == 0
      ? 0
      : x == 1
      ? 1
      : x < 0.5
      ? -(powf(2, 20 * x - 10) * sinf((20 * x - 11.125) * c5)) / 2
      : (powf(2, -20 * x + 10) * sinf((20 * x - 11.125) * c5)) / 2 + 1;
  }

  float inBounce(float x) {
    return 1 - outBounce(1 - x);
  }
  float outBounce(float x) {
    static const float n1 = 7.5625;
    static const float d1 = 2.75;

    if (x < 1 / d1) {
        return n1 * x * x;
    } else if (x < 2 / d1) {
        return n1 * (x - 1.5 / d1) * (x - 1.5) + 0.75;
    } else if (x < 2.5 / d1) {
        return n1 * (x - 2.25 / d1) * (x - 2.25) + 0.9375;
    } else {
        return n1 * (x - 2.625 / d1) * (x - 2.625) + 0.984375;
    }
  }
  float inOutBounce(float x) {
    return x < 0.5
      ? (inBounce(2 * x)) / 2
      : (1 + outBounce(2 * x - 1)) / 2;
  }
}
