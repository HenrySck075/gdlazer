#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

class Interpolation {
public:
  // Returns the interpolated value
  template<typename TValue>
  static TValue valueAt(float time, TValue startValue, TValue endValue, float startTime, float endTime);
  template<typename TValue, typename TEasing>
  static TValue valueAtEasing(float time, TValue startValue, TValue endValue, float startTime, float endTime, TEasing easing);
};
