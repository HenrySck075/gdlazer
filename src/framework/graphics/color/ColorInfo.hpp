#pragma once
#include "../Color4.hpp"

struct ColorInfo {
  Color4 tl;
  Color4 bl;
  Color4 tr;
  Color4 br;
  bool hasSingleColor;

  static ColorInfo singleColor(Color4 c) {
    return {c,c,c,c,true};
  }
  static ColorInfo gradientHorizontal(Color4 c1, Color4 c2) {
    return {c1,c1,c2,c2,false};
  }
  static ColorInfo gradientVertical(Color4 c1, Color4 c2) {
    return {c1,c2,c1,c2,false};
  }
};
