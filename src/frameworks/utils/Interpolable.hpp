#pragma once

#include "../../macro.h"
#include "../transforms/EasingFunction.hpp"

GDF_NS_START
template<typename T>
struct Interpolable {
  /// Causes a link error if the interpolable function is not defined :troll:
  T valueAt(double time, T startValue, T endValue, double startTime, double endTime, IEasingFunction easing);
};
GDF_NS_END
