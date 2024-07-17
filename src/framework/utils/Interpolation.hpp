#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

class Interpolation {
  template<typename TValue, typename TEasing>
  TValue valueAt(double time, TValue startValue, TValue endValue, double startTime, double endTime, TEasing easing);
};
