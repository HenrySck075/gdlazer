#pragma once

#include "../../macro.h"
#include "../graphics/Easings.hpp"
#include <cmath>

GDF_NS_START
struct Interpolation {
  static inline double lerp(double start, double end, double amount) {
    return start + (end - start) * amount;
  }
  static double damp(double start, double final, double base, double exponent) {
    if (base < 0 || 1 < base) {
      throw std::out_of_range(fmt::format("[Interpolation::damp]: base has to lie in [0,1] but is {}", base));
    }
    return lerp(start, final, powf(base, exponent));
  }
  template<typename T>
  static T valueAt(double time, T startValue, T endValue, double startTime, double endTime) {
  }
};
GDF_NS_END
