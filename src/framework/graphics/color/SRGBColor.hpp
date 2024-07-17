#pragma once

#include "../Color4.hpp"

struct SRGBColor {
  Color4 srgb;

  operator Color4() {return srgb;}
};
