#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

class Interpolation {
public:
  static float Lerp(float start, float final, float amount) {
    return start + (final - start) * amount;
  }
  static float Damp(float start, float final, float base, float exponent)
  {
    if (base < 0 || base > 1)
      throw std::out_of_range(fmt::format("base value has to lie in [0,1], but is {}.",base));

    return Lerp(start, final, 1 - powf(base, exponent));
  }
  // Returns the interpolated value
  template<typename TValue>
  static TValue valueAt(float time, TValue startValue, TValue endValue, float startTime, float endTime);
  template<typename TValue, typename TEasing>
  static TValue valueAtEasing(float time, TValue startValue, TValue endValue, float startTime, float endTime, TEasing easing);
};
