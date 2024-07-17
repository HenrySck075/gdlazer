#include "Interpolation.hpp"
#include <type_traits>

template<typename TValue, typename TEasing>
TValue Interpolation::valueAtEasing(float time, TValue startValue, TValue endValue, float startTime, float endTime, TEasing easing) {

  if (std::is_same_v<TValue, int>) {

  }
};
