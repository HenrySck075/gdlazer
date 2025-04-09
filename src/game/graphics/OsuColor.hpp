// Source: https://github.com/ppy/osu/blob/5633297d1c04a539abb0a46b921b4a2c951631e8/osu.Game/Graphics/Osucolor.cs

#pragma once

#include <Geode/Geode.hpp>
#include <cmath>
#include "../../frameworks/graphics/color/Color4.hpp"
#include "../../frameworks/graphics/color/ColorInfo.hpp"

#include "../scoring/ScoreRank.hpp"
#include "../scoring/HitResult.hpp"
#include "../scoring/RankingTier.hpp"
#include "../beatmaps/BeatmapOnlineStatus.hpp"
#include "../rulesets/mods/ModType.hpp"
#include "../online/rooms/RoomCategory.hpp"

#include "../../frameworks/utils/Interpolation.hpp"
using namespace geode::prelude;

GDL_NS_START
struct GradientPoint {
  float position;
  Color4 color;
};

class OsuColor {
public:
  static Color4 Gray(GLubyte amt) {return Color4(amt, amt, amt, 255);};
  Color4 SampleFromLinearGradient(std::vector<GradientPoint> gradient, float point)
    {
      if (point < gradient[0].position)
        return gradient[0].color;

      for (int i = 0; i < gradient.size() - 1; i++)
      {
        auto startStop = gradient[i];
        auto endStop = gradient[i + 1];

        if (point >= endStop.position)
          continue;

        return frameworks::Interpolation::valueAt(point, startStop.color, endStop.color, startStop.position, endStop.position);
      }

      return gradient[-1].color;
    }
  /// <summary>
  /// Retrieves the color for a given point in the star range.
  /// </summary>
  Color4 forStarDifficulty(double starDifficulty) {return SampleFromLinearGradient(
  {
    {0.1f, Color4::fromHex("aaaaaa")},
    {0.1f, Color4::fromHex("4290fb")},
    {1.25f, Color4::fromHex("4fc0ff")},
    {2.0f, Color4::fromHex("4fffd5")},
    {2.5f, Color4::fromHex("7cff4f")},
    {3.3f, Color4::fromHex("f6f05c")},
    {4.2f, Color4::fromHex("ff8068")},
    {4.9f, Color4::fromHex("ff4e6f")},
    {5.8f, Color4::fromHex("c645b8")},
    {6.7f, Color4::fromHex("6563de")},
    {7.7f, Color4::fromHex("18158e")},
    {9.0f, Color4::Black}
  }, (float)ceil(starDifficulty));};

  /// <summary>
  /// Retrieves the color for a <see cref="ScoreRank"/>.
  /// </summary>
  static Color4 forRank(ScoreRank rank)
  {
    switch (rank)
    {
      case ScoreRank::XH:
      case ScoreRank::X:
        return Color4::fromHex("de31ae");

      case ScoreRank::SH:
      case ScoreRank::S:
        return Color4::fromHex("02b5c3");

      case ScoreRank::A:
        return Color4::fromHex("88da20");

      case ScoreRank::B:
        return Color4::fromHex("e3b130");

      case ScoreRank::C:
        return Color4::fromHex("ff8e5d");

      case ScoreRank::D:
        return Color4::fromHex("ff5a5a");

      case ScoreRank::F:
      default:
        return Color4::fromHex("3f3f3f");
    }
  }

  /// <summary>
  /// Retrieves the color for a <see cref="HitResult"/>.
  /// </summary>
  Color4 forHitResult(HitResult result)
  {
    switch (result)
    {
      case HitResult::IgnoreMiss:
      case HitResult::SmallTickMiss:
        return Color4::Gray;

      case HitResult::Miss:
      case HitResult::LargeTickMiss:
      case HitResult::ComboBreak:
        return Color4::Red;

      case HitResult::Meh:
        return Color4::Yellow;

      case HitResult::Ok:
        return Color4::Green;

      case HitResult::Good:
        return Color4::LightGreen;

      case HitResult::SmallTickHit:
      case HitResult::LargeTickHit:
      case HitResult::SliderTailHit:
      case HitResult::Great:
        return Color4::Blue;

      default:
        return Color4::LightBlue;
    }
  }

  /// <summary>
  /// Retrieves a color for the given <see cref="BeatmapOnlineStatus"/>.
  /// A <see langword="null"/> value indicates that a "background" shade from the local <see cref="OverlaycolorProvider"/>
  /// (or another fallback color) should be used.
  /// </summary>
  /// <remarks>
  /// Sourced from web: https://github.com/ppy/osu-web/blob/007eebb1916ed5cb6a7866d82d8011b1060a945e/resources/assets/less/layout.less#L36-L50
  /// </remarks>
  static Color4 forBeatmapSetOnlineStatus(BeatmapOnlineStatus status)
  {
    switch (status)
    {
      case BeatmapOnlineStatus::LocallyModified:
        return Color4::OrangeRed;

      case BeatmapOnlineStatus::Ranked:
      case BeatmapOnlineStatus::Approved:
        return Color4::fromHex("b3ff66");

      case BeatmapOnlineStatus::Loved:
        return Color4::fromHex("ff66ab");

      case BeatmapOnlineStatus::Qualified:
        return Color4::fromHex("66ccff");

      case BeatmapOnlineStatus::Pending:
        return Color4::fromHex("ffd966");

      case BeatmapOnlineStatus::WIP:
        return Color4::fromHex("ff9966");

      case BeatmapOnlineStatus::Graveyard:
        return Color4::Black;

      default:
        return Color4::Black;
    }
  }

  /// <summary>
  /// Retrieves the main accent color for a <see cref="ModType"/>.
  /// </summary>
  Color4 forModType(ModType modType)
  {
    switch (modType)
    {
      case ModType::Automation:
        return Blue1;

      case ModType::DifficultyIncrease:
        return Red1;

      case ModType::DifficultyReduction:
        return Lime1;

      case ModType::Conversion:
        return Purple1;

      case ModType::Fun:
        return Pink1;

      case ModType::System:
        return Yellow;

      default:
        throw "Unknown mod type";
    }
  }

  /// <summary>
  /// Retrieves the main accent color for a <see cref="RoomCategory"/>.
  /// </summary>
  Color4 forRoomCategory(RoomCategory roomCategory)
  {
    //Color4Defined c;
    switch (roomCategory)
    {
    case RoomCategory::Spotlight:
      return SpotlightColor;

    case RoomCategory::FeaturedArtist:
      return FeaturedArtistColor;

    default:
      return Color4::Black;
    }
  }

  /// <summary>
  /// Retrieves color for a <see cref="RankingTier"/>.
  /// See https://www.figma.com/file/YHWhp9wZ089YXgB7pe6L1k/Tier-colors
  /// </summary>
  ColorInfo forRankingTier(RankingTier tier)
  {
    switch (tier)
    {
      default:
      case RankingTier::Iron:
        return ColorInfo::singleColor(Color4::fromHex("BAB3AB"));

      case RankingTier::Bronze:
        return ColorInfo::gradientVertical(Color4::fromHex("B88F7A"), Color4::fromHex("855C47"));

      case RankingTier::Silver:
        return ColorInfo::gradientVertical(Color4::fromHex("E0E0EB"), Color4::fromHex("A3A3C2"));

      case RankingTier::Gold:
        return ColorInfo::gradientVertical(Color4::fromHex("F0E4A8"), Color4::fromHex("E0C952"));

      case RankingTier::Platinum:
        return ColorInfo::gradientVertical(Color4::fromHex("A8F0EF"), Color4::fromHex("52E0DF"));

      case RankingTier::Rhodium:
        return ColorInfo::gradientVertical(Color4::fromHex("D9F8D3"), Color4::fromHex("A0CF96"));

      case RankingTier::Radiant:
        return ColorInfo::gradientVertical(Color4::fromHex("97DCFF"), Color4::fromHex("ED82FF"));

      case RankingTier::Lustrous:
        return ColorInfo::gradientVertical(Color4::fromHex("FFE600"), Color4::fromHex("ED82FF"));
    }
  }

  /// <summary>
  /// Returns a foreground text color that is supposed to contrast well with
  /// the supplied <paramref name="backgroundcolor"/>.
  /// </summary>
  static Color4 foregroundTextcolorFor(Color4 backgroundcolor)
  {
    // formula taken from the RGB->YIQ conversions: https://en.wikipedia.org/wiki/YIQ
    // brightness here is equivalent to the Y component in the above color model, which is a rough estimate of lightness.
    float brightness = 0.299f * backgroundcolor.r + 0.587f * backgroundcolor.g + 0.114f * backgroundcolor.b;
    return Gray(brightness > 0.5f ? 0.2f : 0.9f);
  }

  static Color4 TeamColorRed;
  static Color4 TeamColorBlue;

  // See https://github.com/ppy/osu-web/blob/master/resources/assets/less/colors.less
  static Color4 PurpleLighter;
  static Color4 PurpleLight;
  static Color4 PurpleLightAlternative;
  static Color4 Purple;
  static Color4 PurpleDark;
  static Color4 PurpleDarkAlternative;
  static Color4 PurpleDarker;

  static Color4 PinkLighter;
  static Color4 PinkLight;
  static Color4 Pink;
  static Color4 PinkDark;
  static Color4 PinkDarker;

  static Color4 BlueLighter;
  static Color4 BlueLight;
  static Color4 Blue;
  static Color4 BlueDark;
  static Color4 BlueDarker;

  static Color4 YellowLighter;
  static Color4 YellowLight;
  static Color4 Yellow;
  static Color4 YellowDark;
  static Color4 YellowDarker;

  static Color4 GreenLighter;
  static Color4 GreenLight;
  static Color4 Green;
  static Color4 GreenDark;
  static Color4 GreenDarker;

  static Color4 Sky;
  static Color4 GreySkyLighter;
  static Color4 GreySkyLight;
  static Color4 GreySky;
  static Color4 GreySkyDark;
  static Color4 GreySkyDarker;

  static Color4 SeaFoam;
  static Color4 GreySeaFoamLighter;
  static Color4 GreySeaFoamLight;
  static Color4 GreySeaFoam;
  static Color4 GreySeaFoamDark;
  static Color4 GreySeaFoamDarker;

  static Color4 Cyan;
  static Color4 GreyCyanLighter;
  static Color4 GreyCyanLight;
  static Color4 GreyCyan;
  static Color4 GreyCyanDark;
  static Color4 GreyCyanDarker;

  static Color4 Lime;
  static Color4 GreyLimeLighter;
  static Color4 GreyLimeLight;
  static Color4 GreyLime;
  static Color4 GreyLimeDark;
  static Color4 GreyLimeDarker;

  static Color4 Violet;
  static Color4 GreyVioletLighter;
  static Color4 GreyVioletLight;
  static Color4 GreyViolet;
  static Color4 GreyVioletDark;
  static Color4 GreyVioletDarker;

  static Color4 Carmine;
  static Color4 GreyCarmineLighter;
  static Color4 GreyCarmineLight;
  static Color4 GreyCarmine;
  static Color4 GreyCarmineDark;
  static Color4 GreyCarmineDarker;

  static Color4 Gray0;
  static Color4 Gray1;
  static Color4 Gray2;
  static Color4 Gray3;
  static Color4 Gray4;
  static Color4 Gray5;
  static Color4 Gray6;
  static Color4 Gray7;
  static Color4 Gray8;
  static Color4 Gray9;
  static Color4 GrayA;
  static Color4 GrayB;
  static Color4 GrayC;
  static Color4 GrayD;
  static Color4 GrayE;
  static Color4 GrayF;


  // Reference: https://www.figma.com/file/VIkXMYNPMtQem2RJg9k2iQ/Asset%2Fcolors?node-id=1838%3A3

  // Note that the colors in this region are also defined in `OverlaycolorProvider` as `color{0,1,2,3,4}`.
  // The difference as to which should be used where comes down to context.
  // If the color in question is supposed to always match the view in which it is displayed theme-wise, use `OverlaycolorProvider`.
  // If the color usage is special and in general differs from the surrounding view in choice of hue, use the `Osucolor` constants.

  static Color4 Pink0;
  static Color4 Pink1;
  static Color4 Pink2;
  static Color4 Pink3;
  static Color4 Pink4;

  static Color4 Purple0;
  static Color4 Purple1;
  static Color4 Purple2;
  static Color4 Purple3;
  static Color4 Purple4;

  static Color4 Blue0;
  static Color4 Blue1;
  static Color4 Blue2;
  static Color4 Blue3;
  static Color4 Blue4;

  static Color4 Green0;
  static Color4 Green1;
  static Color4 Green2;
  static Color4 Green3;
  static Color4 Green4;

  static Color4 Lime0;
  static Color4 Lime1;
  static Color4 Lime2;
  static Color4 Lime3;
  static Color4 Lime4;

  static Color4 Orange0;
  static Color4 Orange1;
  static Color4 Orange2;
  static Color4 Orange3;
  static Color4 Orange4;

  static Color4 Red0;
  static Color4 Red1;
  static Color4 Red2;
  static Color4 Red3;
  static Color4 Red4;

  // Content Background
  static Color4 B5;

  static Color4 RedLighter;
  static Color4 RedLight;
  static Color4 Red;
  static Color4 RedDark;
  static Color4 RedDarker;

  static Color4 ChatBlue;

  static Color4 ContextMenuGray;

  static Color4 SpotlightColor;
  static Color4 FeaturedArtistColor;

  static Color4 DangerousButtonColor;
};
GDL_NS_END