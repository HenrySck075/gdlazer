#include "DefaultEasingFunc.hpp"

using namespace gdlazer::frameworks;

const double elastic_const = 2 * M_PI / .3;
const double elastic_const2 = .3 / 4;

const double back_const = 1.70158;
const double back_const2 = back_const * 1.525;

const double bounce_const = 1 / 2.75;

// constants used to fix expo and elastic curves to start/end at 0/1
static const double expo_offset = pow(2, -10);
static const double elastic_offset_full = pow(2, -11);
static const double elastic_offset_half = pow(2, -10) * sin((.5 - elastic_const2) * elastic_const);
static const double elastic_offset_quarter = pow(2, -10) * sin((.25 - elastic_const2) * elastic_const);
static const double in_out_elastic_offset = pow(2, -10) * sin((1 - elastic_const2 * 1.5) * elastic_const / 1.5);

/// Uses the osuframework impl because apparently their elastic function differs from easings.net (which is what easings ns functions use)
double DefaultEasingFunction::applyEasing(double time) {
  switch (m_easing) {
    default:
      return time;

    case Easing::In:
    case Easing::InQuad:
      return time * time;

    case Easing::Out:
    case Easing::OutQuad:
      return time * (2 - time);

    case Easing::InOutQuad:
      if (time < 0.5) return time * time * 2;
      return --time * time * -2 + 1;

    case Easing::InCubic:
      return time * time * time;

    case Easing::OutCubic:
      return --time * time * time + 1;

    case Easing::InOutCubic:
      if (time < 0.5) return time * time * time * 4;
      return --time * time * time * 4 + 1;

    case Easing::InQuart:
      return time * time * time * time;

    case Easing::OutQuart:
      return 1 - --time * time * time * time;

    case Easing::InOutQuart:
      if (time < 0.5) return time * time * time * time * 8;
      return --time * time * time * time * -8 + 1;

    case Easing::InQuint:
      return time * time * time * time * time;

    case Easing::OutQuint:
      return --time * time * time * time * time + 1;

    case Easing::InOutQuint:
      if (time < 0.5) return time * time * time * time * time * 16;
      return --time * time * time * time * time * 16 + 1;

    case Easing::InSine:
      return 1 - cos(time * M_PI * 0.5);

    case Easing::OutSine:
      return sin(time * M_PI * 0.5);

    case Easing::InOutSine:
      return 0.5 - 0.5 * cos(M_PI * time);

    case Easing::InExpo:
      return pow(2, 10 * (time - 1)) + expo_offset * (time - 1);

    case Easing::OutExpo:
      return -pow(2, -10 * time) + 1 + expo_offset * time;

    case Easing::InOutExpo:
      if (time < 0.5) return 0.5 * (pow(2, 20 * time - 10) + expo_offset * (2 * time - 1));
      return 1 - 0.5 * (pow(2, -20 * time + 10) + expo_offset * (-2 * time + 1));

    case Easing::InCirc:
      return 1 - sqrt(1 - time * time);

    case Easing::OutCirc:
      return sqrt(1 - --time * time);

    case Easing::InOutCirc:
      time *= 2;
      if (time < 1) return 0.5 - 0.5 * sqrt(1 - time * time);
      return 0.5 * sqrt(1 - (time -= 2) * time) + 0.5;

    case Easing::InElastic:
      return -pow(2, -10 + 10 * time) * sin((1 - elastic_const2 - time) * elastic_const) + elastic_offset_full * (1 - time);

    case Easing::OutElastic:
      return pow(2, -10 * time) * sin((time - elastic_const2) * elastic_const) + 1 - elastic_offset_full * time;

    case Easing::OutElasticHalf:
      return pow(2, -10 * time) * sin((0.5 * time - elastic_const2) * elastic_const) + 1 - elastic_offset_half * time;

    case Easing::OutElasticQuarter:
      return pow(2, -10 * time) * sin((0.25 * time - elastic_const2) * elastic_const) + 1 - elastic_offset_quarter * time;

    case Easing::InOutElastic:
      time *= 2;
      if (time < 1) {
        return -0.5 * (pow(2, -10 + 10 * time) * sin((1 - elastic_const2 * 1.5 - time) * elastic_const / 1.5)
          - in_out_elastic_offset * (1 - time));
      }
      return 0.5 * (pow(2, -10 * --time) * sin((time - elastic_const2 * 1.5) * elastic_const / 1.5)
        - in_out_elastic_offset * time) + 1;

    case Easing::InBack:
      return time * time * ((back_const + 1) * time - back_const);

    case Easing::OutBack:
      return --time * time * ((back_const + 1) * time + back_const) + 1;

    case Easing::InOutBack:
      time *= 2;
      if (time < 1) return 0.5 * time * time * ((back_const2 + 1) * time - back_const2);
      return 0.5 * ((time -= 2) * time * ((back_const2 + 1) * time + back_const2) + 2);

    case Easing::InBounce:
      time = 1 - time;
      if (time < bounce_const)
        return 1 - 7.5625 * time * time;
      if (time < 2 * bounce_const)
        return 1 - (7.5625 * (time -= 1.5 * bounce_const) * time + 0.75);
      if (time < 2.5 * bounce_const)
        return 1 - (7.5625 * (time -= 2.25 * bounce_const) * time + 0.9375);
      return 1 - (7.5625 * (time -= 2.625 * bounce_const) * time + 0.984375);

    case Easing::OutBounce:
      if (time < bounce_const)
        return 7.5625 * time * time;
      if (time < 2 * bounce_const)
        return 7.5625 * (time -= 1.5 * bounce_const) * time + 0.75;
      if (time < 2.5 * bounce_const)
        return 7.5625 * (time -= 2.25 * bounce_const) * time + 0.9375;
      return 7.5625 * (time -= 2.625 * bounce_const) * time + 0.984375;

    case Easing::InOutBounce: {
      if (time < 0.5) {
        DefaultEasingFunction outBounce(Easing::OutBounce);
        return 0.5 - 0.5 * outBounce.applyEasing(1 - time * 2);
      }
      DefaultEasingFunction outBounce(Easing::OutBounce);
      return outBounce.applyEasing((time - 0.5) * 2) * 0.5 + 0.5;
    }
    case Easing::OutPow10:
      return --time * pow(time, 10) + 1;
  }
}

