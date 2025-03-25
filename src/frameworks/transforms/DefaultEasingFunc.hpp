#pragma once

#include "EasingFunction.hpp"
#include "../graphics/Easings.hpp"

GDF_NS_START
namespace transforms {
  struct DefaultEasingFunction : public IEasingFunction {
    graphics::Easing m_easing;
    double applyEasing(double time) override;
  };
}
GDF_NS_END
