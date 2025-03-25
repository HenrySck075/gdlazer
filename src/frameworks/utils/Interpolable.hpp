#pragma once

#include "../../macro.h"
#include "../graphics/Easings.hpp"

GDF_NS_START
namespace utils {
  template<typename T>
  struct Interpolable {
    /// Causes a link error if the interpolable function is not defined :troll:
    T valueAt(double time, T startValue, T endValue, double startTime, double endTime);
  };
}
GDF_NS_END
