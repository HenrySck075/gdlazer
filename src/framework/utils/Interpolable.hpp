
template<typename TValue, typename TEasing>
class IInterpolable {
  virtual TValue valueAt(double time, TValue startValue, TValue endValue, double startTime, double endTime, TEasing easing) = 0;
};


