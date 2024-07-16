#include "OverlayColorProvider.hpp"
#include "Geode/loader/Log.hpp"
#include "ccTypes.h"
#include <cmath>
#include <cstdlib>

namespace osu {
  namespace utils {
    ccColor3B hslToRgb(float h, float s, float l) {
      float c = (1-abs(2*l-1))*s;
      float x = c * (1 - abs(fmod(h/60,2) - 1));
      float m = l-c/2;
      
      float r_,g_,b_;
      if      (0 <= h && h < 60)    {r_=c;g_=x;b_=0;}
      else if (60 <= h && h < 120)  {r_=x;g_=c;b_=0;}
      else if (120 <= h && h < 180) {r_=0;g_=c;b_=x;}
      else if (180 <= h && h < 240) {r_=0;g_=x;b_=c;}
      else if (240 <= h && h < 300) {r_=x;g_=0;b_=c;}
      else if (300 <= h && h < 360) {r_=c;g_=0;b_=x;}
      
      return ccc3((r_+m)*255, (g_+m)*255, (b_+m)*255);
    }
  };
  namespace Game {
    namespace Overlays {
      float OverlayColorProvider::getBaseHue(OverlayColorScheme colorScheme) {
        switch (colorScheme) {
          default:
              throw "{colorScheme} color scheme does not provide a hue value in getBaseHue.";

          case osu::Game::Overlays::OverlayColorScheme::Red:
              return 0;

          case osu::Game::Overlays::OverlayColorScheme::Pink:
              return 333;

          case osu::Game::Overlays::OverlayColorScheme::Orange:
              return 45;

          case osu::Game::Overlays::OverlayColorScheme::Lime:
              return 90;

          case osu::Game::Overlays::OverlayColorScheme::Green:
              return 125;

          case osu::Game::Overlays::OverlayColorScheme::Aquamarine:
              return 160;

          case osu::Game::Overlays::OverlayColorScheme::Prple:
              return 255;

          case osu::Game::Overlays::OverlayColorScheme::Blue:
              return 200;

          case osu::Game::Overlays::OverlayColorScheme::Plum:
              return 320;
        }
      }
    }
  }
}
