#pragma once

#include "../../macro.h"

GDF_NS_START
namespace transforms {
  struct IEasingFunction {
    virtual double applyEasing(double time) = 0;
  };
}
GDF_NS_END
