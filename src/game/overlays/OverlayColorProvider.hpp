// Source: https://github.com/ppy/osu/blob/54cf64d5ceed8a05aeba44620c5964fa284f5ca7/osu.Game/Overlays/OverlayColourProvider.cs#L10
#ifndef __osu_ui_color_overlay_provider__
#define __osu_ui_color_overlay_provider__


#include <Geode/Geode.hpp>
using namespace geode::prelude;

ccColor3B hslToRgb(float h, float s, float l); 
    
enum OverlayColorScheme
{
  Red,
  Pink,
  Orange,
  Lime,
  Green,
  Prple,
  Blue,
  Plum,
  Aquamarine
};
class OverlayColorProvider : public CCObject{
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
    ccColor4B Color0() {return getColor(1, 0.8f);};
    ccColor4B Color1() {return getColor(1, 0.7f);};
    ccColor4B Color2() {return getColor(0.8f, 0.6f);};
    ccColor4B Color3() {return getColor(0.6f, 0.5f);};
    ccColor4B Color4() {return getColor(0.4f, 0.3f);};

    ccColor4B Highlight1() {return getColor(1, 0.7f);};
    ccColor4B Content1() {return getColor(0.4f, 1);};
    ccColor4B Content2() {return getColor(0.4f, 0.9f);};
    ccColor4B Light1() {return getColor(0.4f, 0.8f);};
    ccColor4B Light2() {return getColor(0.4f, 0.75f);};
    ccColor4B Light3() {return getColor(0.4f, 0.7f);};
    ccColor4B Light4() {return getColor(0.4f, 0.5f);};
    ccColor4B Dark1() {return getColor(0.2f, 0.35f);};
    ccColor4B Dark2() {return getColor(0.2f, 0.3f);};
    ccColor4B Dark3() {return getColor(0.2f, 0.25f);};
    ccColor4B Dark4() {return getColor(0.2f, 0.2f);};
    ccColor4B Dark5() {return getColor(0.2f, 0.15f);};
    ccColor4B Dark6() {return getColor(0.2f, 0.1f);};
    ccColor4B Foreground1() {return getColor(0.1f, 0.6f);};
    ccColor4B Background1() {return getColor(0.1f, 0.4f);};
    ccColor4B Background2() {return getColor(0.1f, 0.3f);};
    ccColor4B Background3() {return getColor(0.1f, 0.25f);};
    ccColor4B Background4() {return getColor(0.1f, 0.2f);};
    ccColor4B Background5() {return getColor(0.1f, 0.15f);};
    ccColor4B Background6() {return getColor(0.1f, 0.1f);};

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
    ccColor4B getColor(float saturation, float lightness) {
      auto c = hslToRgb(getBaseHue(ColorScheme), saturation, lightness);
      return ccc4(c.r, c.g, c.b, 255);
    };

    // See https://github.com/ppy/osu-web/blob/5a536d217a21582aad999db50a981003d3ad5659/app/helpers.php#L1620-L1628
    static float getBaseHue(OverlayColorScheme colorScheme); 
};

#endif // !__osu_ui_color_overlay_provider__
