#pragma once

#include "../../macro.h"

GDF_NS_START
struct IEasingFunction {
  virtual double applyEasing(double time) {return time;};
};
GDF_NS_END
