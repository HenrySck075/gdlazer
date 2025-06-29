/// @note Interface is from MusicController in the osu!game repo

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/cocos/include/cocos2d.h>

#include "../bindables/Event.hpp"

GDF_NS_START
struct MusicEnded : public Event {};
struct MusicStarted : public Event {};
struct MusicInfoRequest : public Event {};

/// FMODAudioEngine 2
///
/// Inheriting CCNode is for the ActionManager, 
/// which requires a CCNode as a target just because some CCActions uses CCNode as a target
/// 
/// but still why is GJGameLevel inherits CCNode
class AudioManager : public cocos2d::CCNode {
  GameLevelManager* m_levelManager;
  GJGameLevel* m_currentLevel;
  LevelTools* m_tools;

  // there will always be someone spamming level select

  gd::string m_songName = "";
  gd::string m_songAuthor = "";
  gd::string m_levelName = "";
  gd::string m_levelAuthor = "";

  FMOD::System* m_sys;
  FMOD::Channel* m_channel;
  FMOD::DSP* m_dsp;
  FMOD::DSP* m_lowpassdsp;
  FMOD::Sound* m_sound;

  bool m_paused = false;
  bool m_ended = false;
  float m_lowPassStrength = 0;

  void onSongEnd();
  void updateLowPassFilter();
public:
  FMOD::DSP* getDSP() {return m_dsp;}

  static AudioManager* get();
  bool init();

  void set(gd::string filePath, float fadeTime);
  void play() {
    m_paused = false;
    m_channel->setPaused(false);
  };
  void playFromLevel(GJGameLevel* level, float fadeTime);
  void stop() {
    m_paused = true;
    m_channel->setPaused(true);
  };

  void dispatchMusicInfoEvent(Container* target);

  void setLowPassStrength(float lps);

  inline gd::string getSongName() {return m_songName;}
  inline gd::string getSongAuthor() {return m_songAuthor;}
  inline gd::string getLevelName() {return m_levelName;}
  inline gd::string getLevelAuthor() {return m_levelAuthor;}

  float getSongDuration();
  float getElapsed();
  void seek(float position);

  friend FMOD_RESULT fmodSoundCallback(
    FMOD_CHANNELCONTROL*,
    FMOD_CHANNELCONTROL_TYPE ,
    FMOD_CHANNELCONTROL_CALLBACK_TYPE,
    void *, void *
  );
};
GDF_NS_END