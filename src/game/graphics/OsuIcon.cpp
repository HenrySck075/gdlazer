// definition file


#include "OsuIcon.hpp"
std::string unicode_to_utf8(int unicode)
{
    std::string s;

    if (unicode>=0 and unicode <= 0x7f)  // 7F(16) = 127(10)
    {
        s = static_cast<char>(unicode);

        return s;
    }
    else if (unicode <= 0x7ff)  // 7FF(16) = 2047(10)
    {
        unsigned char c1 = 192, c2 = 128;

        for (int k=0; k<11; ++k)
        {
            if (k < 6)
                c2 |= (unicode % 64) & (1 << k);
            else
                c1 |= (unicode >> 6) & (1 << (k - 6));
        }

        s = c1;
        s += c2;

        return s;
    }
    else if (unicode <= 0xffff)  // FFFF(16) = 65535(10)
    {
        unsigned char c1 = 224, c2 = 128, c3 = 128;

        for (int k=0; k<16; ++k)
        {
            if (k < 6)
                c3 |= (unicode % 64) & (1 << k);
            else if
                (k < 12) c2 |= (unicode >> 6) & (1 << (k - 6));
            else
                c1 |= (unicode >> 12) & (1 << (k - 12));
        }

        s = c1;
        s += c2;
        s += c3;

        return s;
    }
    else if (unicode <= 0x1fffff)  // 1FFFFF(16) = 2097151(10)
    {
        unsigned char c1 = 240, c2 = 128, c3 = 128, c4 = 128;

        for (int k=0; k<21; ++k)
        {
            if (k < 6)
                c4 |= (unicode % 64) & (1 << k);
            else if (k < 12)
                c3 |= (unicode >> 6) & (1 << (k - 6));
            else if (k < 18)
                c2 |= (unicode >> 12) & (1 << (k - 12));
            else
                c1 |= (unicode >> 18) & (1 << (k - 18));
        }

        s = c1;
        s += c2;
        s += c3;
        s += c4;

        return s;
    }
    else if (unicode <= 0x3ffffff)  // 3FFFFFF(16) = 67108863(10)
    {
        ;  // Actually, there are no 5-bytes unicodes
    }
    else if (unicode <= 0x7fffffff)  // 7FFFFFFF(16) = 2147483647(10)
    {
        ;  // Actually, there are no 6-bytes unicodes
    }
    else
        ;  // Incorrect unicode (< 0 or > 2147483647)

    return "";
}

/*
#define iconFont(name, charid, fontname) IconConstructor OsuIcon::name = {unicode_to_utf8(charid).c_str(),fontname}
#define iconOsuFont(name, charid) iconFont(name,charid,"osuFont.fnt"_spr)
#define iconOsuIcon(name, charid) iconFont(name,charid,"osuIcon.fnt"_spr)
#define iconOsuEditor(name, charid) iconFont(name,charid,"osuEditor.fnt"_spr)
    // ruleset icons without circles
    // my guy why tf did you call these ruleset
    iconOsuFont(FilledCircle, 0xe004);
    iconOsuFont(Logo, 0xe006);
    iconOsuFont(ChevronDownCircle, 0xe007);
    iconOsuFont(EditCircle, 0xe033);
    iconOsuFont(LeftCircle, 0xe034);
    iconOsuFont(RightCircle, 0xe035);
    iconOsuFont(Charts, 0xe036);
    iconOsuFont(Solo, 0xe037);
    iconOsuFont(Multi, 0xe038);
    iconOsuFont(Gear, 0xe039);

    // misc icons
    iconOsuFont(Bat, 0xe008);
    iconOsuFont(Bubble, 0xe009);
    iconOsuFont(BubblePop, 0xe02e);
    iconOsuFont(Dice, 0xe011);
    iconOsuFont(HeartBreak, 0xe030);
    iconOsuFont(Hot, 0xe031);
    iconOsuFont(ListSearch, 0xe032);

    //osu! playstyles
    iconOsuFont(PlayStyleTablet, 0xe02a);
    iconOsuFont(PlayStyleMouse, 0xe029);
    iconOsuFont(PlayStyleKeyboard, 0xe02b);
    iconOsuFont(PlayStyleTouch, 0xe02c);

    // osu! difficulties
    iconOsuFont(EasyOsu, 0xe015);
    iconOsuFont(NormalOsu, 0xe016);
    iconOsuFont(HardOsu, 0xe017);
    iconOsuFont(InsaneOsu, 0xe018);
    iconOsuFont(ExpertOsu, 0xe019);

    // taiko difficulties
    iconOsuFont(EasyTaiko, 0xe01a);
    iconOsuFont(NormalTaiko, 0xe01b);
    iconOsuFont(HardTaiko, 0xe01c);
    iconOsuFont(InsaneTaiko, 0xe01d);
    iconOsuFont(ExpertTaiko, 0xe01e);

    // fruits difficulties
    iconOsuFont(EasyFruits, 0xe01f);
    iconOsuFont(NormalFruits, 0xe020);
    iconOsuFont(HardFruits, 0xe021);
    iconOsuFont(InsaneFruits, 0xe022);
    iconOsuFont(ExpertFruits, 0xe023);

    // mania difficulties
    iconOsuFont(EasyMania, 0xe024);
    iconOsuFont(NormalMania, 0xe025);
    iconOsuFont(HardMania, 0xe026);
    iconOsuFont(InsaneMania, 0xe027);
    iconOsuFont(ExpertMania, 0xe028);

    // mod icons
    iconOsuFont(ModPerfect, 0xe049);
    iconOsuFont(ModAutopilot, 0xe03a);
    iconOsuFont(ModAuto, 0xe03b);
    iconOsuFont(ModCinema, 0xe03c);
    iconOsuFont(ModDoubleTime, 0xe03d);
    iconOsuFont(ModEasy, 0xe03e);
    iconOsuFont(ModFlashlight, 0xe03f);
    iconOsuFont(ModHalftime, 0xe040);
    iconOsuFont(ModHardRock, 0xe041);
    iconOsuFont(ModHidden, 0xe042);
    iconOsuFont(ModNightcore, 0xe043);
    iconOsuFont(ModNoFail, 0xe044);
    iconOsuFont(ModRelax, 0xe045);
    iconOsuFont(ModSpunOut, 0xe046);
    iconOsuFont(ModSuddenDeath, 0xe047);
    iconOsuFont(ModTarget, 0xe048);

    // single-file icons but geode just places everything in the root folder so i just put them in a font
    iconOsuIcon(Audio, 58001);
    iconOsuIcon(Beatmap, 58002);
    iconOsuIcon(Calendar, 58003);
    iconOsuIcon(ChangelogA, 58004);
    iconOsuIcon(ChangelogB, 58005);
    iconOsuIcon(Chat, 58006);
    iconOsuIcon(CheckCircle, 58007);
    iconOsuIcon(CollapseA, 58008);
    iconOsuIcon(Collections, 58009);
    iconOsuIcon(Cross, 58011);
    iconOsuIcon(CrossCircle, 58010);
    iconOsuIcon(Crown, 58012);
    iconOsuIcon(DailyChallenge, 58013);
    iconOsuIcon(Debug, 58014);
    iconOsuIcon(Delete, 58015);
    iconOsuIcon(Details, 58016);
    iconOsuIcon(Discord, 58017);
    iconOsuIcon(EllipsisHorizontal, 58018);
    iconOsuIcon(EllipsisVertical, 58019);
    iconOsuIcon(ExpandA, 58020);
    iconOsuIcon(ExpandB, 58021);
    iconOsuIcon(FeaturedArtist, 58022);
    iconOsuIcon(FeaturedArtistCircle, 58023);
    iconOsuIcon(GameplayA, 58024);
    iconOsuIcon(GameplayB, 58025);
    iconOsuIcon(GameplayC, 58026);
    iconOsuIcon(Global, 58027);
    iconOsuIcon(Graphics, 58028);
    iconOsuIcon(Heart, 58030);
    iconOsuIcon(Home, 58031);
    iconOsuIcon(Input, 58032);
    iconOsuIcon(Maintenance, 58033);
    iconOsuIcon(Megaphone, 58034);
    iconOsuIcon(Music, 58035);
    iconOsuIcon(News, 58036);
    iconOsuIcon(Next, 58037);
    iconOsuIcon(NextCircle, 58038);
    iconOsuIcon(Notification, 58039);
    iconOsuIcon(Online, 58040);
    iconOsuIcon(Play, 58041);
    iconOsuIcon(Player, 58043);
    iconOsuIcon(PlayerFollow, 58042);
    iconOsuIcon(Prev, 58045);
    iconOsuIcon(PrevCircle, 58044);
    iconOsuIcon(Ranking, 58046);
    iconOsuIcon(Rulesets, 58047);
    iconOsuIcon(Search, 58048);
    iconOsuIcon(Settings, 58049);
    iconOsuIcon(SkinA, 58050);
    iconOsuIcon(SkinB, 58051);
    iconOsuIcon(Star, 58052);
    iconOsuIcon(Storyboard, 58053);
    iconOsuIcon(Team, 58054);
    iconOsuIcon(ThumbsUp, 58055);
    iconOsuIcon(Tournament, 58057);
    iconOsuIcon(Twitter, 58058);
    iconOsuIcon(UserInterface, 58059);
    iconOsuIcon(Wiki, 58060);
    iconOsuEditor(EditorAddControlPoint, 58101);
    iconOsuEditor(EditorConvertToStream, 58102);
    iconOsuEditor(EditorDistanceSnap, 58103);
    iconOsuEditor(EditorFinish, 58104);
    iconOsuEditor(EditorGridSnap, 58105);
    iconOsuEditor(EditorNewComboA, 58106);
    iconOsuEditor(EditorNewComboB, 58107);
    iconOsuEditor(EditorSelect, 58108);
    iconOsuEditor(EditorSound, 58109);
    iconOsuEditor(EditorWhistle, 58110);
    iconOsuIcon(Tortoise, 58056);
    iconOsuIcon(Hare, 58029);

#undef iconFont
#undef iconOsuFont
#undef iconOsuIcon
#undef iconOsuEditor

*/