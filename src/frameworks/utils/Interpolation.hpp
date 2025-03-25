#pragma once

#include "../../macro.h"
#include "../graphics/Easings.hpp"

GDF_NS_START
namespace utils {
  struct Interpolation {
    static double lerp(double start, double end, double amount) {
      return start + (end - start) * amount;
    }
    /*
    template<typename T>
    static T valueAt(double time, T startValue, T endValue, double startTime, double endTime) {
    }
    */
  };
}
GDF_NS_END
