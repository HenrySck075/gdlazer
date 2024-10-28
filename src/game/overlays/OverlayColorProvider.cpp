#include "OverlayColorProvider.hpp"
#include "ccTypes.h"
#include <cmath>
#include <cstdlib>

ccColor3B hslToRgb(float h, float s, float l) {
  float c = (1-abs(2*l-1))*s;
  float x = c * (1 - abs(fmod(h/60,2) - 1));
  float m = l-c/2;
  
  float r_,g_,b_;
  if    (0 <= h && h < 60)  {r_=c;g_=x;b_=0;}
  else if (60 <= h && h < 120)  {r_=x;g_=c;b_=0;}
  else if (120 <= h && h < 180) {r_=0;g_=c;b_=x;}
  else if (180 <= h && h < 240) {r_=0;g_=x;b_=c;}
  else if (240 <= h && h < 300) {r_=x;g_=0;b_=c;}
  else if (300 <= h && h < 360) {r_=c;g_=0;b_=x;}
  
  return ccc3((r_+m)*255, (g_+m)*255, (b_+m)*255);
}
float OverlayColorProvider::getBaseHue(OverlayColorScheme colorScheme) {
  switch (colorScheme) {
  default:
    throw "{colorScheme} color scheme does not provide a hue value in getBaseHue.";

  case OverlayColorScheme::Red:
    return 0;

  case OverlayColorScheme::Pink:
    return 333;

  case OverlayColorScheme::Orange:
    return 45;

  case OverlayColorScheme::Lime:
    return 90;

  case OverlayColorScheme::Green:
    return 125;

  case OverlayColorScheme::Aquamarine:
    return 160;

  case OverlayColorScheme::Purple:
    return 255;

  case OverlayColorScheme::Blue:
    return 200;

  case OverlayColorScheme::Plum:
    return 320;
  }
}
