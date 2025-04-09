#include "GenericInterpolables.hpp"
#include "Interpolation.hpp"
GDF_NS_START
Color4 Interpolable<Color4>::valueAt(double time, Color4 startColor, Color4 endColor, double startTime, double endTime, IEasingFunction easing) {
  if (startColor == endColor)
      return startColor;

  double current = time - startTime;
  double duration = endTime - startTime;

  if (duration == 0 || current == 0)
      return startColor;

  Color4 startLinear = startColor.toLinear();
  Color4 endLinear = endColor.toLinear();

  float t = fmaxf(0, fminf(1, (float)easing.applyEasing(current / duration)));

  return Color4(
    Interpolation::lerp(startLinear.r, endLinear.r, t),
    Interpolation::lerp(startLinear.g, endLinear.g, t),
    Interpolation::lerp(startLinear.b, endLinear.b, t),
    Interpolation::lerp(startLinear.a, endLinear.a, t)
  );
};
GDF_NS_END