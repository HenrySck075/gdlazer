#include "AudioManager.hpp"
#include <Geode/cocos/actions/CCActionManager.h>
#include "../../helpers/CustomActions.hpp"
#include "../Game.hpp"
#include <random>

static gdlazer::frameworks::AudioManager* instance;
GDF_NS_START
AudioManager* AudioManager::get() {
  if (!instance) {
    instance = new AudioManager();
    if (!instance->init()) delete instance;
  }
  return instance;
};

bool AudioManager::init() {
  m_sys = FMODAudioEngine::sharedEngine()->m_system;
  FMOD::ChannelGroup* masterChannel = FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel;
  m_tools = new LevelTools();
  
  m_sys->createDSPByType(FMOD_DSP_TYPE_FFT, &m_dsp);
  masterChannel->addDSP(2, m_dsp);

  m_sys->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &m_lowpassdsp);
  //masterChannel->addDSP(7, m_lowpassdsp);
  m_lowpassdsp->setParameterFloat(FMOD_DSP_LOWPASS_RESONANCE, 0.f);

  return true;
}

void AudioManager::setLowPassStrength(float lps) {
  m_lowPassStrength = lps;
  updateLowPassFilter();
}
void AudioManager::updateLowPassFilter() {
  auto master = FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel;
  if (m_lowPassStrength <= 0) {
    master->removeDSP(m_lowpassdsp);
  } else {
    master->addDSP(7, m_lowpassdsp);
    m_lowpassdsp->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, m_lowPassStrength);
  }
  /*
  */
}

/** TODO: Some tracks plays "Stereo Madness" but actually uses a custom song
 (it doesnt play stereo madness)
*/
void AudioManager::playFromLevel(GJGameLevel* level, float fadeTime) {
  if (level == nullptr) return;
  m_currentLevel = level;
  int track = 1;
  SongInfoObject* customSongInfo = nullptr;
  if (!level->m_songIDs.empty()) {
    std::string s = level->m_songIDs;
    std::vector<std::string> songIDs;
    std::string delim = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
      token = s.substr(0, pos);
      songIDs.push_back(token);
      s.erase(0, pos + delim.length());
    }
    songIDs.push_back(s);

    std::vector<std::string> out;
    std::sample(
      songIDs.begin(),
      songIDs.end(),
      std::back_inserter(out),
      1,
      std::mt19937{std::random_device{}()}
    );
    
    track = std::stoi(out[0]);
    customSongInfo = MusicDownloadManager::sharedState()->getSongInfoObject(track);
  } else {
    if (level->m_isUploaded) track = level->m_songID;
    else track = level->m_audioTrack;
  }

  m_songName = m_currentLevel
    ? (
      customSongInfo == nullptr
      ? m_tools->getAudioTitle(track) 
      : customSongInfo->m_songName
    )
    : "The Most Mysterious Song On The Internet";
  m_songAuthor = m_currentLevel
    ? (
      customSongInfo == nullptr
      ? m_tools->nameForArtist(m_tools->artistForAudio(track))
      : customSongInfo->m_artistName
    )
    : "Unknown";
  m_levelName = m_currentLevel?m_currentLevel->m_levelName:"";
  m_levelAuthor = m_currentLevel?m_currentLevel->m_creatorName:"";

  log::debug(
    "Playing \"{}\" by {} (id: {}), used in \"{}\" by {}", 
    m_songName, m_songAuthor, track, m_levelName, m_levelAuthor
  );

  set(
    CCFileUtils::sharedFileUtils()->fullPathForFilename(
      level->getAudioFileName().c_str(),false
    ),
    fadeTime
  );
}

FMOD_RESULT F_CALLBACK fmodSoundCallback(
  FMOD_CHANNELCONTROL* channel,
  FMOD_CHANNELCONTROL_TYPE type,
  FMOD_CHANNELCONTROL_CALLBACK_TYPE cbType,
  void *, void *
) {
  if (cbType == FMOD_CHANNELCONTROL_CALLBACK_END) {
    log::debug("[fmodSoundCallback()]: song ended");
    if (type != FMOD_CHANNELCONTROL_CHANNEL) return FMOD_RESULT::FMOD_OK;
    if(instance) instance->onSongEnd();
  }
  return FMOD_RESULT::FMOD_OK;
};

void AudioManager::onSongEnd() {
  m_ended = true;
  log::debug("[AudioManager]: song ended, dispatching music ended event");
  Game::get()->dispatchEvent(new MusicEnded());
}

void AudioManager::set(gd::string filePath, float fadeTime) {
  unsigned int prevFadePoint = 0;
  auto playNewSound = [this,filePath,fadeTime,&prevFadePoint]{
    if (m_sound) m_sound->release();
    m_channel->setChannelGroup(nullptr);
    m_channel->stop();
    m_sys->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &m_sound);
    m_sys->playSound(m_sound,nullptr,true,&m_channel);
    seek(0);
    m_channel->setCallback(&fmodSoundCallback);
    m_channel->setChannelGroup(FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel);
    m_ended = false;
    if (fadeTime>0) {
      // fading
      unsigned int* pos;
      // remove fadePoints set by the function below this
      m_channel->removeFadePoints(prevFadePoint,prevFadePoint+fadeTime*1000);
      // seek forward current+fadeTime to
      m_channel->setPosition(fadeTime*1000,FMOD_TIMEUNIT_MS);
      // get the fade end position
      m_channel->getPosition(pos,FMOD_TIMEUNIT_PCM);
      // seek it back
      m_channel->setPosition(0,FMOD_TIMEUNIT_MS);
      // add fade points
      m_channel->addFadePoint(0, 0);
      m_channel->addFadePoint((unsigned long long)pos, 1);
    }
    m_channel->setPaused(m_paused);
    Game::get()->dispatchEvent(new MusicStarted());
  };
  if (m_sound && fadeTime>0 && !m_ended) {
    runAction(cocos2d::CCSequence::create(
      CCCallFuncL::create([this,fadeTime,&prevFadePoint]{
        unsigned int pos = 0;
        unsigned int posMS = 0;
        // get the current position
        m_channel->getPosition(&posMS,FMOD_TIMEUNIT_MS);
        prevFadePoint = posMS;
        // do the exact same as what playNewSound did, except with the current position
        m_channel->setPosition(posMS+fadeTime*1000,FMOD_TIMEUNIT_MS);
        m_channel->getPosition(&pos,FMOD_TIMEUNIT_PCM);
        m_channel->setPosition(posMS,FMOD_TIMEUNIT_MS);
        m_channel->addFadePoint((unsigned long long)posMS, 1);
        m_channel->addFadePoint((unsigned long long)pos, 0);
      }),
      CCDelayTime::create(fadeTime),
      CCCallFuncL::create(playNewSound),
      nullptr
    ));
  } else {playNewSound();}
}
GDF_NS_END