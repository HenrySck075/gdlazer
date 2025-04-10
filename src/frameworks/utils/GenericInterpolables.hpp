#pragma once

////////////////////////////////////////////////////////////
// Interpolable functions for several gd!framework classes
////////////////////////////////////////////////////////////

#include "Interpolable.hpp"
#include "../graphics/color/Color4.hpp"
GDF_NS_START

template<>
struct Interpolable<Color4> {
  Color4 valueAt(double time, Color4 startValue, Color4 endValue, double startTime, double endTime, IEasingFunction easing);
};

GDF_NS_END
