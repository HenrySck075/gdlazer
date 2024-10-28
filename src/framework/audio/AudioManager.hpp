/// @note Interface is from MusicController in the osu!game repo

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/cocos/include/cocos2d.h>

#include "../bindables/Event.hpp"

namespace {
  extern char const musicEnded[] = "musicEnded";
  extern char const musicStarted[] = "musicStarted";
}
using MusicEnded = NamedNodeEvent<musicEnded>;
using MusicStarted = NamedNodeEvent<musicStarted>;

/// FMODAudioEngine 2
///
/// Inheriting CCNode is for the ActionManager, 
/// which requires a CCNode as a target just because some CCActions uses CCNode as a target
/// 
/// but still why is GJGameLevel inherits CCNode
class AudioManager : public cocos2d::CCNode {
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
  bool ended = false;

  void onSongEnd();
public:
  FMOD::DSP* getDSP() {return dsp;}

  static AudioManager* get();
  bool init();

  void set(gd::string filePath, float fadeTime);
  void play() {
    paused = false;
    channel->setPaused(false);
  };
  void playFromLevel(GJGameLevel* level, float fadeTime);
  void stop() {
    paused = true;
    channel->setPaused(true);
  };

  inline gd::string getSongName() {return songName;}
  inline gd::string getSongAuthor() {return songAuthor;}
  inline gd::string getLevelName() {return levelName;}
  inline gd::string getLevelAuthor() {return levelAuthor;}

  float getSongDuration() {
    if (!sound) return 0;
    unsigned int ret;
    sound->getLength(&ret, FMOD_TIMEUNIT_MS);
    return ret/1000;
  }
  void seek(float position) {
    if (channel) channel->setPosition((unsigned int)(position*1000), FMOD_TIMEUNIT_MS);
  }

  friend FMOD_RESULT fmodSoundCallback(
    FMOD_CHANNELCONTROL*,
    FMOD_CHANNELCONTROL_TYPE ,
    FMOD_CHANNELCONTROL_CALLBACK_TYPE,
    void *, void *
  );
};
