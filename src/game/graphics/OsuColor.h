// Source: https://github.com/ppy/osu/blob/5633297d1c04a539abb0a46b921b4a2c951631e8/osu.Game/Graphics/Osucolor.cs

#include <Geode/Geode.hpp>
#include <cmath>
#include "../../helpers/ColorHelper.hpp"
#include "../scoring/ScoreRank.hpp"
#include "../scoring/HitResult.hpp"
using namespace geode::prelude;
using Color4 = ccColor4B;

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

                return Interpolation.ValueAt(point, startStop.color, endStop.color, startStop.position, endStop.position);
            }

            return gradient[-1].color;
        }
    /// <summary>
    /// Retrieves the color for a given point in the star range.
    /// </summary>
    Color4 forStarDifficulty(double starDifficulty) {return SampleFromLinearGradient(
    {
        {0.1f, CCColorExtension::color4FromHex(0xaaaaaaff)},
        {0.1f, CCColorExtension::color4FromHex(0x4290fbff)},
        {1.25f, CCColorExtension::color4FromHex(0x4fc0ffff)},
        {2.0f, CCColorExtension::color4FromHex(0x4fffd5ff)},
        {2.5f, CCColorExtension::color4FromHex(0x7cff4fff)},
        {3.3f, CCColorExtension::color4FromHex(0xf6f05cff)},
        {4.2f, CCColorExtension::color4FromHex(0xff8068ff)},
        {4.9f, CCColorExtension::color4FromHex(0xff4e6fff)},
        {5.8f, CCColorExtension::color4FromHex(0xc645b8ff)},
        {6.7f, CCColorExtension::color4FromHex(0x6563deff)},
        {7.7f, CCColorExtension::color4FromHex(0x18158eff)},
        {9.0f, ccc4(0, 0, 0, 1)}
    }, (float)ceil(starDifficulty));};

    /// <summary>
    /// Retrieves the color for a <see cref="ScoreRank"/>.
    /// </summary>
    static Color4 forRank(ScoreRank rank)
    {
        switch (rank)
        {
            case ScoreRank.XH:
            case ScoreRank.X:
                return CCColorExtension::color4FromHex(0xde31aeff);

            case ScoreRank.SH:
            case ScoreRank.S:
                return CCColorExtension::color4FromHex(0x02b5c3ff);

            case ScoreRank.A:
                return CCColorExtension::color4FromHex(0x88da20ff);

            case ScoreRank.B:
                return CCColorExtension::color4FromHex(0xe3b130ff);

            case ScoreRank.C:
                return CCColorExtension::color4FromHex(0xff8e5dff);

            case ScoreRank.D:
                return CCColorExtension::color4FromHex(0xff5a5aff);

            case ScoreRank.F:
            default:
                return CCColorExtension::color4FromHex(0x3f3f3fff);
        }
    }

    /// <summary>
    /// Retrieves the color for a <see cref="HitResult"/>.
    /// </summary>
    Color4 forHitResult(HitResult result)
    {
        switch (result)
        {
            case HitResult.IgnoreMiss:
            case HitResult.SmallTickMiss:
                return Color4.Gray;

            case HitResult.Miss:
            case HitResult.LargeTickMiss:
            case HitResult.ComboBreak:
                return Red;

            case HitResult.Meh:
                return Yellow;

            case HitResult.Ok:
                return Green;

            case HitResult.Good:
                return GreenLight;

            case HitResult.SmallTickHit:
            case HitResult.LargeTickHit:
            case HitResult.SliderTailHit:
            case HitResult.Great:
                return Blue;

            default:
                return BlueLight;
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
            case BeatmapOnlineStatus.LocallyModified:
                return Color4.OrangeRed;

            case BeatmapOnlineStatus.Ranked:
            case BeatmapOnlineStatus.Approved:
                return CCColorExtension::color4FromHex(0xb3ff66ff);

            case BeatmapOnlineStatus.Loved:
                return CCColorExtension::color4FromHex(0xff66abff);

            case BeatmapOnlineStatus.Qualified:
                return CCColorExtension::color4FromHex(0x66ccffff);

            case BeatmapOnlineStatus.Pending:
                return CCColorExtension::color4FromHex(0xffd966ff);

            case BeatmapOnlineStatus.WIP:
                return CCColorExtension::color4FromHex(0xff9966ff);

            case BeatmapOnlineStatus.Graveyard:
                return Color4.Black;

            default:
                return null;
        }
    }

    /// <summary>
    /// Retrieves the main accent color for a <see cref="ModType"/>.
    /// </summary>
    Color4 forModType(ModType modType)
    {
        switch (modType)
        {
            case ModType.Automation:
                return Blue1;

            case ModType.DifficultyIncrease:
                return Red1;

            case ModType.DifficultyReduction:
                return Lime1;

            case ModType.Conversion:
                return Purple1;

            case ModType.Fun:
                return Pink1;

            case ModType.System:
                return Yellow;

            default:
                throw new ArgumentOutOfRangeException(nameof(modType), modType, "Unknown mod type");
        }
    }

    /// <summary>
    /// Retrieves the main accent color for a <see cref="RoomCategory"/>.
    /// </summary>
    Color4 forRoomCategory(RoomCategory roomCategory)
    {
        switch (roomCategory)
        {
            case RoomCategory.Spotlight:
                return Spotlightcolor;

            case RoomCategory.FeaturedArtist:
                return FeaturedArtistcolor;

            default:
                return null;
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
                return CCColorExtension::color4FromHex(0xBAB3ABff);

            case RankingTier.Bronze:
                return colorInfo.GradientVertical(CCColorExtension::color4FromHex(0xB88F7Aff), CCColorExtension::color4FromHex(0x855C47ff);

            case RankingTier.Silver:
                return colorInfo.GradientVertical(CCColorExtension::color4FromHex(0xE0E0EBff), CCColorExtension::color4FromHex(0xA3A3C2ff);

            case RankingTier.Gold:
                return colorInfo.GradientVertical(CCColorExtension::color4FromHex(0xF0E4A8ff), CCColorExtension::color4FromHex(0xE0C952ff);

            case RankingTier.Platinum:
                return colorInfo.GradientVertical(CCColorExtension::color4FromHex(0xA8F0EFff), CCColorExtension::color4FromHex(0x52E0DFff);

            case RankingTier.Rhodium:
                return colorInfo.GradientVertical(CCColorExtension::color4FromHex(0xD9F8D3ff), CCColorExtension::color4FromHex(0xA0CF96ff);

            case RankingTier.Radiant:
                return colorInfo.GradientVertical(CCColorExtension::color4FromHex(0x97DCFFff), CCColorExtension::color4FromHex(0xED82FFff);

            case RankingTier.Lustrous:
                return colorInfo.GradientVertical(CCColorExtension::color4FromHex(0xFFE600ff), CCColorExtension::color4FromHex(0xED82FFff);
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

    Color4 TeamcolorRed = CCColorExtension::color4FromHex(0xAA1414ff);
    Color4 TeamcolorBlue = CCColorExtension::color4FromHex(0x1462AAff);

    // See https://github.com/ppy/osu-web/blob/master/resources/assets/less/colors.less
    Color4 PurpleLighter = CCColorExtension::color4FromHex(0xeeeeffff);
    Color4 PurpleLight = CCColorExtension::color4FromHex(0xaa88ffff);
    Color4 PurpleLightAlternative = CCColorExtension::color4FromHex(0xcba4daff);
    Color4 Purple = CCColorExtension::color4FromHex(0x8866eeff);
    Color4 PurpleDark = CCColorExtension::color4FromHex(0x6644ccff);
    Color4 PurpleDarkAlternative = CCColorExtension::color4FromHex(0x312436ff);
    Color4 PurpleDarker = CCColorExtension::color4FromHex(0x441188ff);

    Color4 PinkLighter = CCColorExtension::color4FromHex(0xffddeeff);
    Color4 PinkLight = CCColorExtension::color4FromHex(0xff99ccff);
    Color4 Pink = CCColorExtension::color4FromHex(0xff66aaff);
    Color4 PinkDark = CCColorExtension::color4FromHex(0xcc5288ff);
    Color4 PinkDarker = CCColorExtension::color4FromHex(0xbb1177ff);

    Color4 BlueLighter = CCColorExtension::color4FromHex(0xddffffff);
    Color4 BlueLight = CCColorExtension::color4FromHex(0x99eeffff);
    Color4 Blue = CCColorExtension::color4FromHex(0x66ccffff);
    Color4 BlueDark = CCColorExtension::color4FromHex(0x44aaddff);
    Color4 BlueDarker = CCColorExtension::color4FromHex(0x2299bbff);

    Color4 YellowLighter = CCColorExtension::color4FromHex(0xffffddff);
    Color4 YellowLight = CCColorExtension::color4FromHex(0xffdd55ff);
    Color4 Yellow = CCColorExtension::color4FromHex(0xffcc22ff);
    Color4 YellowDark = CCColorExtension::color4FromHex(0xeeaa00ff);
    Color4 YellowDarker = CCColorExtension::color4FromHex(0xcc6600ff);

    Color4 GreenLighter = CCColorExtension::color4FromHex(0xeeffccff);
    Color4 GreenLight = CCColorExtension::color4FromHex(0xb3d944ff);
    Color4 Green = CCColorExtension::color4FromHex(0x88b300ff);
    Color4 GreenDark = CCColorExtension::color4FromHex(0x668800ff);
    Color4 GreenDarker = CCColorExtension::color4FromHex(0x445500ff);

    Color4 Sky = CCColorExtension::color4FromHex(0x6bb5ffff);
    Color4 GreySkyLighter = CCColorExtension::color4FromHex(0xc6e3f4ff);
    Color4 GreySkyLight = CCColorExtension::color4FromHex(0x8ab3ccff);
    Color4 GreySky = CCColorExtension::color4FromHex(0x405461ff);
    Color4 GreySkyDark = CCColorExtension::color4FromHex(0x303d47ff);
    Color4 GreySkyDarker = CCColorExtension::color4FromHex(0x21272cff);

    Color4 SeaFoam = CCColorExtension::color4FromHex(0x05ffa2ff);
    Color4 GreySeaFoamLighter = CCColorExtension::color4FromHex(0x9ebab1ff);
    Color4 GreySeaFoamLight = CCColorExtension::color4FromHex(0x4d7365ff);
    Color4 GreySeaFoam = CCColorExtension::color4FromHex(0x33413cff);
    Color4 GreySeaFoamDark = CCColorExtension::color4FromHex(0x2c3532ff);
    Color4 GreySeaFoamDarker = CCColorExtension::color4FromHex(0x1e2422ff);

    Color4 Cyan = CCColorExtension::color4FromHex(0x05f4fdff);
    Color4 GreyCyanLighter = CCColorExtension::color4FromHex(0x77b1b3ff);
    Color4 GreyCyanLight = CCColorExtension::color4FromHex(0x436d6fff);
    Color4 GreyCyan = CCColorExtension::color4FromHex(0x293d3eff);
    Color4 GreyCyanDark = CCColorExtension::color4FromHex(0x243536ff);
    Color4 GreyCyanDarker = CCColorExtension::color4FromHex(0x1e2929ff);

    Color4 Lime = CCColorExtension::color4FromHex(0x82ff05ff);
    Color4 GreyLimeLighter = CCColorExtension::color4FromHex(0xdeff87ff);
    Color4 GreyLimeLight = CCColorExtension::color4FromHex(0x657259ff);
    Color4 GreyLime = CCColorExtension::color4FromHex(0x3f443aff);
    Color4 GreyLimeDark = CCColorExtension::color4FromHex(0x32352eff);
    Color4 GreyLimeDarker = CCColorExtension::color4FromHex(0x2e302bff);

    Color4 Violet = CCColorExtension::color4FromHex(0xbf04ffff);
    Color4 GreyVioletLighter = CCColorExtension::color4FromHex(0xebb8feff);
    Color4 GreyVioletLight = CCColorExtension::color4FromHex(0x685370ff);
    Color4 GreyViolet = CCColorExtension::color4FromHex(0x46334dff);
    Color4 GreyVioletDark = CCColorExtension::color4FromHex(0x2c2230ff);
    Color4 GreyVioletDarker = CCColorExtension::color4FromHex(0x201823ff);

    Color4 Carmine = CCColorExtension::color4FromHex(0xff0542ff);
    Color4 GreyCarmineLighter = CCColorExtension::color4FromHex(0xdeaab4ff);
    Color4 GreyCarmineLight = CCColorExtension::color4FromHex(0x644f53ff);
    Color4 GreyCarmine = CCColorExtension::color4FromHex(0x342b2dff);
    Color4 GreyCarmineDark = CCColorExtension::color4FromHex(0x302a2bff);
    Color4 GreyCarmineDarker = CCColorExtension::color4FromHex(0x241d1eff);

    Color4 Gray0 = CCColorExtension::color4FromHex(0x000ff);
    Color4 Gray1 = CCColorExtension::color4FromHex(0x111ff);
    Color4 Gray2 = CCColorExtension::color4FromHex(0x222ff);
    Color4 Gray3 = CCColorExtension::color4FromHex(0x333ff);
    Color4 Gray4 = CCColorExtension::color4FromHex(0x444ff);
    Color4 Gray5 = CCColorExtension::color4FromHex(0x555ff);
    Color4 Gray6 = CCColorExtension::color4FromHex(0x666ff);
    Color4 Gray7 = CCColorExtension::color4FromHex(0x777ff);
    Color4 Gray8 = CCColorExtension::color4FromHex(0x888ff);
    Color4 Gray9 = CCColorExtension::color4FromHex(0x999ff);
    Color4 GrayA = CCColorExtension::color4FromHex(0xaaaff);
    Color4 GrayB = CCColorExtension::color4FromHex(0xbbbff);
    Color4 GrayC = CCColorExtension::color4FromHex(0xcccff);
    Color4 GrayD = CCColorExtension::color4FromHex(0xdddff);
    Color4 GrayE = CCColorExtension::color4FromHex(0xeeeff);
    Color4 GrayF = CCColorExtension::color4FromHex(0xfffff);


    // Reference: https://www.figma.com/file/VIkXMYNPMtQem2RJg9k2iQ/Asset%2Fcolors?node-id=1838%3A3

    // Note that the colors in this region are also defined in `OverlaycolorProvider` as `color{0,1,2,3,4}`.
    // The difference as to which should be used where comes down to context.
    // If the color in question is supposed to always match the view in which it is displayed theme-wise, use `OverlaycolorProvider`.
    // If the color usage is special and in general differs from the surrounding view in choice of hue, use the `Osucolor` constants.

    Color4 Pink0 = CCColorExtension::color4FromHex(0xff99c7ff);
    Color4 Pink1 = CCColorExtension::color4FromHex(0xff66abff);
    Color4 Pink2 = CCColorExtension::color4FromHex(0xeb4791ff);
    Color4 Pink3 = CCColorExtension::color4FromHex(0xcc3378ff);
    Color4 Pink4 = CCColorExtension::color4FromHex(0x6b2e49ff);

    Color4 Purple0 = CCColorExtension::color4FromHex(0xb299ffff);
    Color4 Purple1 = CCColorExtension::color4FromHex(0x8c66ffff);
    Color4 Purple2 = CCColorExtension::color4FromHex(0x7047ebff);
    Color4 Purple3 = CCColorExtension::color4FromHex(0x5933ccff);
    Color4 Purple4 = CCColorExtension::color4FromHex(0x3d2e6bff);

    Color4 Blue0 = CCColorExtension::color4FromHex(0x99ddffff);
    Color4 Blue1 = CCColorExtension::color4FromHex(0x66ccffff);
    Color4 Blue2 = CCColorExtension::color4FromHex(0x47b4ebff);
    Color4 Blue3 = CCColorExtension::color4FromHex(0x3399ccff);
    Color4 Blue4 = CCColorExtension::color4FromHex(0x2e576bff);

    Color4 Green0 = CCColorExtension::color4FromHex(0x99ffa2ff);
    Color4 Green1 = CCColorExtension::color4FromHex(0x66ff73ff);
    Color4 Green2 = CCColorExtension::color4FromHex(0x47eb55ff);
    Color4 Green3 = CCColorExtension::color4FromHex(0x33cc40ff);
    Color4 Green4 = CCColorExtension::color4FromHex(0x2e6b33ff);

    Color4 Lime0 = CCColorExtension::color4FromHex(0xccff99ff);
    Color4 Lime1 = CCColorExtension::color4FromHex(0xb2ff66ff);
    Color4 Lime2 = CCColorExtension::color4FromHex(0x99eb47ff);
    Color4 Lime3 = CCColorExtension::color4FromHex(0x7fcc33ff);
    Color4 Lime4 = CCColorExtension::color4FromHex(0x4c6b2eff);

    Color4 Orange0 = CCColorExtension::color4FromHex(0xffe699ff);
    Color4 Orange1 = CCColorExtension::color4FromHex(0xffd966ff);
    Color4 Orange2 = CCColorExtension::color4FromHex(0xebc247ff);
    Color4 Orange3 = CCColorExtension::color4FromHex(0xcca633ff);
    Color4 Orange4 = CCColorExtension::color4FromHex(0x6b5c2eff);

    Color4 Red0 = CCColorExtension::color4FromHex(0xff9b9bff);
    Color4 Red1 = CCColorExtension::color4FromHex(0xff6666ff);
    Color4 Red2 = CCColorExtension::color4FromHex(0xeb4747ff);
    Color4 Red3 = CCColorExtension::color4FromHex(0xcc3333ff);
    Color4 Red4 = CCColorExtension::color4FromHex(0x6b2e2eff);

    // Content Background
    Color4 B5 = CCColorExtension::color4FromHex(0x222a28ff);

    Color4 RedLighter = CCColorExtension::color4FromHex(0xffededff);
    Color4 RedLight = CCColorExtension::color4FromHex(0xed7787ff);
    Color4 Red = CCColorExtension::color4FromHex(0xed1121ff);
    Color4 RedDark = CCColorExtension::color4FromHex(0xba0011ff);
    Color4 RedDarker = CCColorExtension::color4FromHex(0x870000ff);

    Color4 ChatBlue = CCColorExtension::color4FromHex(0x17292eff);

    Color4 ContextMenuGray = CCColorExtension::color4FromHex(0x223034ff);

    Color4 Spotlightcolor => Green2;
    Color4 FeaturedArtistcolor => Blue2;

    Color4 DangerousButtoncolor => Pink3;
}
