// Source: https://github.com/ppy/osu/blob/5633297d1c04a539abb0a46b921b4a2c951631e8/osu.Game/Graphics/Osucolor.cs

#include <Geode/Geode.hpp>
#include <cmath>
#include "../../framework/graphics/Color4.hpp"
#include "../scoring/ScoreRank.hpp"
#include "../scoring/HitResult.hpp"
#include "../beatmaps/BeatmapOnlineStatus.hpp"
#include "../rulesets/mods/ModType.hpp"
#include "../../framework/utils/Interpolation.hpp"
using namespace geode::prelude;

struct GradientPoint {
  float position;
  Color4 color;
};

class Osucolor {
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

                return Interpolation::valueAt(point, startStop.color, endStop.color, startStop.position, endStop.position);
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
        {9.0f, Color4Defined().Black}
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
        Color4Defined c;
        switch (result)
        {
            case HitResult::IgnoreMiss:
            case HitResult::SmallTickMiss:
                return c.Gray;

            case HitResult::Miss:
            case HitResult::LargeTickMiss:
            case HitResult::ComboBreak:
                return c.Red;

            case HitResult::Meh:
                return c.Yellow;

            case HitResult::Ok:
                return c.Green;

            case HitResult::Good:
                return c.LightGreen;

            case HitResult::SmallTickHit:
            case HitResult::LargeTickHit:
            case HitResult::SliderTailHit:
            case HitResult::Great:
                return c.Blue;

            default:
                return c.LightBlue;
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
                return Color4Defined().OrangeRed;

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
                return Color4Defined().Black;

            default:
                return Color4Defined().Black;
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
      Color4Defined c;
      switch (roomCategory)
      {
        case RoomCategory.Spotlight:
          return SpotlightColor;

        case RoomCategory.FeaturedArtist:
          return FeaturedArtistColor;

        default:
          return c.Black;
      }
    }

    /// <summary>
    /// Retrieves color for a <see cref="RankingTier"/>.
    /// See https://www.figma.com/file/YHWhp9wZ089YXgB7pe6L1k/Tier-colors
    /// </summary>
    colorInfo forRankingTier(RankingTier tier)
    {
        switch (tier)
        {
            default:
            case RankingTier.Iron:
                return Color4::fromHex("BAB3AB");

            case RankingTier.Bronze:
                return colorInfo.GradientVertical(Color4::fromHex("B88F7Aff"), CCColorExtension::color4FromHex("855C47");

            case RankingTier.Silver:
                return colorInfo.GradientVertical(Color4.fromHex("E0E0EBff), CCColorExtension::color4FromHex(0xA3A3C2");

            case RankingTier.Gold:
                return colorInfo.GradientVertical(Color4.fromHex("F0E4A8ff), CCColorExtension::color4FromHex(0xE0C952");

            case RankingTier.Platinum:
                return colorInfo.GradientVertical(Color4.fromHex("A8F0EFff), CCColorExtension::color4FromHex(0x52E0DF");

            case RankingTier.Rhodium:
                return colorInfo.GradientVertical(Color4.fromHex("D9F8D3ff), CCColorExtension::color4FromHex(0xA0CF96");

            case RankingTier.Radiant:
                return colorInfo.GradientVertical(Color4.fromHex("97DCFFff), CCColorExtension::color4FromHex(0xED82FF");

            case RankingTier.Lustrous:
                return colorInfo.GradientVertical(Color4.fromHex("FFE600ff), CCColorExtension::color4FromHex(0xED82FF");
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
        float brightness = 0.299f * backgroundcolor.R + 0.587f * backgroundcolor.G + 0.114f * backgroundcolor.B;
        return Gray(brightness > 0.5f ? 0.2f : 0.9f);
    }

    Color4 TeamcolorRed = Color4::fromHex("AA1414");
    Color4 TeamcolorBlue = Color4::fromHex("1462AA");

    // See https://github.com/ppy/osu-web/blob/master/resources/assets/less/colors.less
    Color4 PurpleLighter = Color4::fromHex("eeeeff");
    Color4 PurpleLight = Color4::fromHex("aa88ff");
    Color4 PurpleLightAlternative = Color4::fromHex("cba4da");
    Color4 Purple = Color4::fromHex("8866ee");
    Color4 PurpleDark = Color4::fromHex("6644cc");
    Color4 PurpleDarkAlternative = Color4::fromHex("312436");
    Color4 PurpleDarker = Color4::fromHex("441188");

    Color4 PinkLighter = Color4::fromHex("ffddee");
    Color4 PinkLight = Color4::fromHex("ff99cc");
    Color4 Pink = Color4::fromHex("ff66aa");
    Color4 PinkDark = Color4::fromHex("cc5288");
    Color4 PinkDarker = Color4::fromHex("bb1177");

    Color4 BlueLighter = Color4::fromHex("ddffff");
    Color4 BlueLight = Color4::fromHex("99eeff");
    Color4 Blue = Color4::fromHex("66ccff");
    Color4 BlueDark = Color4::fromHex("44aadd");
    Color4 BlueDarker = Color4::fromHex("2299bb");

    Color4 YellowLighter = Color4::fromHex("ffffdd");
    Color4 YellowLight = Color4::fromHex("ffdd55");
    Color4 Yellow = Color4::fromHex("ffcc22");
    Color4 YellowDark = Color4::fromHex("eeaa00");
    Color4 YellowDarker = Color4::fromHex("cc6600");

    Color4 GreenLighter = Color4::fromHex("eeffcc");
    Color4 GreenLight = Color4::fromHex("b3d944");
    Color4 Green = Color4::fromHex("88b300");
    Color4 GreenDark = Color4::fromHex("668800");
    Color4 GreenDarker = Color4::fromHex("445500");

    Color4 Sky = Color4::fromHex("6bb5ff");
    Color4 GreySkyLighter = Color4::fromHex("c6e3f4");
    Color4 GreySkyLight = Color4::fromHex("8ab3cc");
    Color4 GreySky = Color4::fromHex("405461");
    Color4 GreySkyDark = Color4::fromHex("303d47");
    Color4 GreySkyDarker = Color4::fromHex("21272c");

    Color4 SeaFoam = Color4::fromHex("05ffa2");
    Color4 GreySeaFoamLighter = Color4::fromHex("9ebab1");
    Color4 GreySeaFoamLight = Color4::fromHex("4d7365");
    Color4 GreySeaFoam = Color4::fromHex("33413c");
    Color4 GreySeaFoamDark = Color4::fromHex("2c3532");
    Color4 GreySeaFoamDarker = Color4::fromHex("1e2422");

    Color4 Cyan = Color4::fromHex("05f4fd");
    Color4 GreyCyanLighter = Color4::fromHex("77b1b3");
    Color4 GreyCyanLight = Color4::fromHex("436d6f");
    Color4 GreyCyan = Color4::fromHex("293d3e");
    Color4 GreyCyanDark = Color4::fromHex("243536");
    Color4 GreyCyanDarker = Color4::fromHex("1e2929");

    Color4 Lime = Color4::fromHex("82ff05");
    Color4 GreyLimeLighter = Color4::fromHex("deff87");
    Color4 GreyLimeLight = Color4::fromHex("657259");
    Color4 GreyLime = Color4::fromHex("3f443a");
    Color4 GreyLimeDark = Color4::fromHex("32352e");
    Color4 GreyLimeDarker = Color4::fromHex("2e302b");

    Color4 Violet = Color4::fromHex("bf04ff");
    Color4 GreyVioletLighter = Color4::fromHex("ebb8fe");
    Color4 GreyVioletLight = Color4::fromHex("685370");
    Color4 GreyViolet = Color4::fromHex("46334d");
    Color4 GreyVioletDark = Color4::fromHex("2c2230");
    Color4 GreyVioletDarker = Color4::fromHex("201823");

    Color4 Carmine = Color4::fromHex("ff0542");
    Color4 GreyCarmineLighter = Color4::fromHex("deaab4");
    Color4 GreyCarmineLight = Color4::fromHex("644f53");
    Color4 GreyCarmine = Color4::fromHex("342b2d");
    Color4 GreyCarmineDark = Color4::fromHex("302a2b");
    Color4 GreyCarmineDarker = Color4::fromHex("241d1e");

    Color4 Gray0 = Color4::fromHex("000");
    Color4 Gray1 = Color4::fromHex("111");
    Color4 Gray2 = Color4::fromHex("222");
    Color4 Gray3 = Color4::fromHex("333");
    Color4 Gray4 = Color4::fromHex("444");
    Color4 Gray5 = Color4::fromHex("555");
    Color4 Gray6 = Color4::fromHex("666");
    Color4 Gray7 = Color4::fromHex("777");
    Color4 Gray8 = Color4::fromHex("888");
    Color4 Gray9 = Color4::fromHex("999");
    Color4 GrayA = Color4::fromHex("aaa");
    Color4 GrayB = Color4::fromHex("bbb");
    Color4 GrayC = Color4::fromHex("ccc");
    Color4 GrayD = Color4::fromHex("ddd");
    Color4 GrayE = Color4::fromHex("eee");
    Color4 GrayF = Color4::fromHex("fff");


    // Reference: https://www.figma.com/file/VIkXMYNPMtQem2RJg9k2iQ/Asset%2Fcolors?node-id=1838%3A3

    // Note that the colors in this region are also defined in `OverlaycolorProvider` as `color{0,1,2,3,4}`.
    // The difference as to which should be used where comes down to context.
    // If the color in question is supposed to always match the view in which it is displayed theme-wise, use `OverlaycolorProvider`.
    // If the color usage is special and in general differs from the surrounding view in choice of hue, use the `Osucolor` constants.

    Color4 Pink0 = Color4::fromHex("ff99c7");
    Color4 Pink1 = Color4::fromHex("ff66ab");
    Color4 Pink2 = Color4::fromHex("eb4791");
    Color4 Pink3 = Color4::fromHex("cc3378");
    Color4 Pink4 = Color4::fromHex("6b2e49");

    Color4 Purple0 = Color4::fromHex("b299ff");
    Color4 Purple1 = Color4::fromHex("8c66ff");
    Color4 Purple2 = Color4::fromHex("7047eb");
    Color4 Purple3 = Color4::fromHex("5933cc");
    Color4 Purple4 = Color4::fromHex("3d2e6b");

    Color4 Blue0 = Color4::fromHex("99ddff");
    Color4 Blue1 = Color4::fromHex("66ccff");
    Color4 Blue2 = Color4::fromHex("47b4eb");
    Color4 Blue3 = Color4::fromHex("3399cc");
    Color4 Blue4 = Color4::fromHex("2e576b");

    Color4 Green0 = Color4::fromHex("99ffa2");
    Color4 Green1 = Color4::fromHex("66ff73");
    Color4 Green2 = Color4::fromHex("47eb55");
    Color4 Green3 = Color4::fromHex("33cc40");
    Color4 Green4 = Color4::fromHex("2e6b33");

    Color4 Lime0 = Color4::fromHex("ccff99");
    Color4 Lime1 = Color4::fromHex("b2ff66");
    Color4 Lime2 = Color4::fromHex("99eb47");
    Color4 Lime3 = Color4::fromHex("7fcc33");
    Color4 Lime4 = Color4::fromHex("4c6b2e");

    Color4 Orange0 = Color4::fromHex("ffe699");
    Color4 Orange1 = Color4::fromHex("ffd966");
    Color4 Orange2 = Color4::fromHex("ebc247");
    Color4 Orange3 = Color4::fromHex("cca633");
    Color4 Orange4 = Color4::fromHex("6b5c2e");

    Color4 Red0 = Color4::fromHex("ff9b9b");
    Color4 Red1 = Color4::fromHex("ff6666");
    Color4 Red2 = Color4::fromHex("eb4747");
    Color4 Red3 = Color4::fromHex("cc3333");
    Color4 Red4 = Color4::fromHex("6b2e2e");

    // Content Background
    Color4 B5 = Color4::fromHex("222a28");

    Color4 RedLighter = Color4::fromHex("ffeded");
    Color4 RedLight = Color4::fromHex("ed7787");
    Color4 Red = Color4::fromHex("ed1121");
    Color4 RedDark = Color4::fromHex("ba0011");
    Color4 RedDarker = Color4::fromHex("870000");

    Color4 ChatBlue = Color4::fromHex("17292e");

    Color4 ContextMenuGray = Color4::fromHex("223034");

    Color4 SpotlightColor = Green2;
    Color4 FeaturedArtistColor = Blue2;

    Color4 DangerousButtonColor = Pink3;
}
