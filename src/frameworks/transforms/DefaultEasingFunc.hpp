#pragma once

#include "EasingFunction.hpp"
#include "../graphics/Easings.hpp"

GDF_NS_START
struct DefaultEasingFunction : public IEasingFunction {
  Easing m_easing;
  DefaultEasingFunction(Easing easing) : m_easing(easing) {}
  double applyEasing(double time) override;
};
GDF_NS_END
