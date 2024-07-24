#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

#define iconFont(name, charid, fontname) CCLabelBMFont* name = CCLabelBMFont::create(std::to_string(charid).c_str(),fontname)
#define iconOsuFont(name, charid) iconFont(name,charid,"osuFont.fnt"_spr)
#define iconOsuIcon(name, charid) iconFont(name,charid,"osuIcon.fnt"_spr)
#define iconOsuEditor(name, charid) iconFont(name,charid,"osuEditor.fnt"_spr)
// are you high
struct OsuIcon {
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
    iconOsuIcon(Audio, 56001);
    iconOsuIcon(Beatmap, 56002);
    iconOsuIcon(Calendar, 56003);
    iconOsuIcon(ChangelogA, 56004);
    iconOsuIcon(ChangelogB, 56005);
    iconOsuIcon(Chat, 56006);
    iconOsuIcon(CheckCircle, 56007);
    iconOsuIcon(CollapseA, 56008);
    iconOsuIcon(Collections, 56009);
    iconOsuIcon(Cross, 56010);
    iconOsuIcon(CrossCircle, 56011);
    iconOsuIcon(Crown, 56012);
    iconOsuIcon(DailyChallenge, 56013);
    iconOsuIcon(Debug, 56014);
    iconOsuIcon(Delete, 56015);
    iconOsuIcon(Details, 56016);
    iconOsuIcon(Discord, 56017);
    iconOsuIcon(EllipsisHorizontal, 56018);
    iconOsuIcon(EllipsisVertical, 56019);
    iconOsuIcon(ExpandA, 56020);
    iconOsuIcon(ExpandB, 56021);
    iconOsuIcon(FeaturedArtist, 56022);
    iconOsuIcon(FeaturedArtistCircle, 56023);
    iconOsuIcon(GameplayA, 56024);
    iconOsuIcon(GameplayB, 56025);
    iconOsuIcon(GameplayC, 56026);
    iconOsuIcon(Global, 56027);
    iconOsuIcon(Graphics, 56028);
    iconOsuIcon(Heart, 56030);
    iconOsuIcon(Home, 56031);
    iconOsuIcon(Input, 56032);
    iconOsuIcon(Maintenance, 56033);
    iconOsuIcon(Megaphone, 56034);
    iconOsuIcon(Music, 56035);
    iconOsuIcon(News, 56036);
    iconOsuIcon(Next, 56037);
    iconOsuIcon(NextCircle, 56038);
    iconOsuIcon(Notification, 56039);
    iconOsuIcon(Online, 56040);
    iconOsuIcon(Play, 56041);
    iconOsuIcon(Player, 56042);
    iconOsuIcon(PlayerFollow, 56043);
    iconOsuIcon(Prev, 56044);
    iconOsuIcon(PrevCircle, 56045);
    iconOsuIcon(Ranking, 56046);
    iconOsuIcon(Rulesets, 56047);
    iconOsuIcon(Search, 56048);
    iconOsuIcon(Settings, 56049);
    iconOsuIcon(SkinA, 56050);
    iconOsuIcon(SkinB, 56051);
    iconOsuIcon(Star, 56052);
    iconOsuIcon(Storyboard, 56053);
    iconOsuIcon(Team, 56054);
    iconOsuIcon(ThumbsUp, 56055);
    iconOsuIcon(Tournament, 56057);
    iconOsuIcon(Twitter, 56058);
    iconOsuIcon(UserInterface, 56059);
    iconOsuIcon(Wiki, 56060);
    iconOsuEditor(EditorAddControlPoint, 56101);
    iconOsuEditor(EditorConvertToStream, 56102);
    iconOsuEditor(EditorDistanceSnap, 56103);
    iconOsuEditor(EditorFinish, 56104);
    iconOsuEditor(EditorGridSnap, 56105);
    iconOsuEditor(EditorNewComboA, 56106);
    iconOsuEditor(EditorNewComboB, 56107);
    iconOsuEditor(EditorSelect, 56108);
    iconOsuEditor(EditorSound, 56109);
    iconOsuEditor(EditorWhistle, 56110);
    iconOsuIcon(Tortoise, 56056);
    iconOsuIcon(Hare, 56029);

};

#undef iconFont
#undef iconOsuFont
#undef iconOsuIcon
#undef iconOsuEditor
