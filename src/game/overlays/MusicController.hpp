#pragma once

#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/cocos/include/cocos2d.h>

/// FMODAudioEngine 2
///
/// Inheriting CCNode is for the ActionManager, 
/// which requires a CCNode as a target just because some CCActions uses CCNode as a target
/// 
/// but still why is GJGameLevel inherits CCNode
class MusicController : public cocos2d::CCNode {
    GameLevelManager* levelManager;
    GJGameLevel* currentLevel;
    LevelTools* tools;

    // there will always be someone spamming level select

    gd::string songName = "";
    gd::string songAuthor = "";
    gd::string levelName = "";
    gd::string levelAuthor = "";

    FMOD::System* sys;
    FMOD::Channel* channel;
    FMOD::DSP* dsp;
    FMOD::Sound* sound;

    bool paused = false;
public:
    static MusicController* get();
    bool init();

    void set(gd::string filePath, float fadeTime = 0.25);
    void play() {
        paused = false;
        channel->setPaused(false);
    };
    void playFromLevel(GJGameLevel* level, float fadeTime = 0.25);
    void stop() {
        paused = true;
        channel->setPaused(true);
    };

    inline gd::string getSongName() {return songName;}
    inline gd::string getSongAuthor() {return songAuthor;}
    inline gd::string getLevelName() {return levelName;}
    inline gd::string getLevelAuthor() {return levelAuthor;}
};