// Source: https://github.com/ppy/osu/blob/54cf64d5ceed8a05aeba44620c5964fa284f5ca7/osu.Game/Overlays/OverlayColourProvider.cs#L10
#pragma once

#include <Geode/Geode.hpp>
#include "../../frameworks/graphics/color/Color4.hpp"
#include "../../macro.h"
using namespace geode::prelude;

GDL_NS_START

using osuColor4 = Color4;
ccColor3B hslToRgb(float h, float s, float l); 
  
enum class OverlayColorScheme {
  Red,
  Pink,
  Orange,
  Lime,
  Green,
  Purple,
  Blue,
  Plum,
  Aquamarine
};
class OverlayColorProvider : public CCObject {
public:
  OverlayColorScheme ColorScheme; 

  static OverlayColorProvider* create(OverlayColorScheme colorScheme)
  {
    auto ret = new OverlayColorProvider();
    if (ret && ret->init(colorScheme)) {
    ret->autorelease();
    } else {
    CC_SAFE_DELETE(ret);
    }
    return ret;
  };

  bool init(OverlayColorScheme colorScheme) {
    ColorScheme = colorScheme;
    return true;
  }

  // Note that the following five colors are also defined in `OsuColor` as `{colorScheme}{0,1,2,3,4}`.
  // The difference as to which should be used where comes down to context.
  // If the color in question is supposed to always match the view in which it is displayed theme-wise, use `OverlayColorProvider`.
  // If the color usage is special and in general differs from the srrounding view in choice of hue, use the `OsuColor` constants.
  osuColor4 Color0() {return getColor(1, 0.8f);};
  osuColor4 Color1() {return getColor(1, 0.7f);};
  osuColor4 Color2() {return getColor(0.8f, 0.6f);};
  osuColor4 Color3() {return getColor(0.6f, 0.5f);};
  osuColor4 Color4() {return getColor(0.4f, 0.3f);};

  osuColor4 Highlight1() {return getColor(1, 0.7f);};
  osuColor4 Content1() {return getColor(0.4f, 1);};
  osuColor4 Content2() {return getColor(0.4f, 0.9f);};
  osuColor4 Light1() {return getColor(0.4f, 0.8f);};
  osuColor4 Light2() {return getColor(0.4f, 0.75f);};
  osuColor4 Light3() {return getColor(0.4f, 0.7f);};
  osuColor4 Light4() {return getColor(0.4f, 0.5f);};
  osuColor4 Dark1() {return getColor(0.2f, 0.35f);};
  osuColor4 Dark2() {return getColor(0.2f, 0.3f);};
  osuColor4 Dark3() {return getColor(0.2f, 0.25f);};
  osuColor4 Dark4() {return getColor(0.2f, 0.2f);};
  osuColor4 Dark5() {return getColor(0.2f, 0.15f);};
  osuColor4 Dark6() {return getColor(0.2f, 0.1f);};
  osuColor4 Foreground1() {return getColor(0.1f, 0.6f);};
  osuColor4 Background1() {return getColor(0.1f, 0.4f);};
  osuColor4 Background2() {return getColor(0.1f, 0.3f);};
  osuColor4 Background3() {return getColor(0.1f, 0.25f);};
  osuColor4 Background4() {return getColor(0.1f, 0.2f);};
  osuColor4 Background5() {return getColor(0.1f, 0.15f);};
  osuColor4 Background6() {return getColor(0.1f, 0.1f);};

  /// <summary>
  /// Changes the value of <see cref="ColorScheme"/> to a different color scheme.
  /// Note that this does not trigger any kind of signal to any drawable that received colors from here, all drawables need to be updated manually.
  /// </summary>
  /// <param name="colorScheme">The proposed color scheme.</param>
  void changeColorScheme(OverlayColorScheme colorScheme)
  {
    ColorScheme = colorScheme;
  }

  private:
  osuColor4 getColor(float saturation, float lightness) {
    auto c = hslToRgb(getBaseHue(ColorScheme), saturation, lightness);
    return {c.r/255.f, c.g/255.f, c.b/255.f, 1.f};
  };

  // See https://github.com/ppy/osu-web/blob/5a536d217a21582aad999db50a981003d3ad5659/app/helpers.php#L1620-L1628
  static float getBaseHue(OverlayColorScheme colorScheme); 
};

GDL_NS_END