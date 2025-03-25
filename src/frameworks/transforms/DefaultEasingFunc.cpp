#include "DefaultEasingFunc.hpp"

using namespace gdlazer::framework;
using namespace gdlazer::framework::graphics;
double transforms::DefaultEasingFunction::applyEasing(double time) {
  switch (m_easing) {
  case Easing::In: [[fallthrough]];
  case Easing::InQuad:
    return easings::inRate(time, 2);

  case Easing::Out: [[fallthrough]];
  case Easing::OutQuad:
    return easings::outRate(time, 2);

  case Easing::InOutQuad:
    return easings::inOutRate(time, 2);

  case Easing::InCubic:
    return easings::inRate(time, 3);
  case Easing::OutCubic:
    return easings::outRate(time, 3);
  case Easing::InOutCubic:
    return easings::inOutRate(time, 3);
  }
}

