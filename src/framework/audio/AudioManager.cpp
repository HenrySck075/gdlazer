#include "AudioManager.hpp"
#include <Geode/cocos/actions/CCActionManager.h>
#include "../../helpers/CustomActions.hpp"
#include "../Game.hpp"
#include <random>

static AudioManager* instance;
AudioManager* AudioManager::get() {
  if (!instance) {
    instance = new AudioManager();
    if (!instance->init()) delete instance;
  }
  return instance;
};

bool AudioManager::init() {
  sys = FMODAudioEngine::sharedEngine()->m_system;
  FMOD::ChannelGroup* masterChannel = FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel;
  tools = new LevelTools();
  
  sys->createDSPByType(FMOD_DSP_TYPE_FFT, &dsp);
  masterChannel->addDSP(2, dsp);

  return true;
}

/** TODO: Some tracks plays "Stereo Madness" but it actually uses a custom song
 (it doesnt play stereo madness)
*/
void AudioManager::playFromLevel(GJGameLevel* level, float fadeTime) {
  currentLevel = level;
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

  songName = currentLevel
    ? (
      customSongInfo == nullptr
      ? tools->getAudioTitle(track) 
      : customSongInfo->m_songName
    )
    : "The Most Mysterious Song On The Internet";
  songAuthor = currentLevel
    ? (
      customSongInfo == nullptr
      ? tools->nameForArtist(tools->artistForAudio(track))
      : customSongInfo->m_artistName
    )
    : "Unknown";
  levelName = currentLevel?currentLevel->m_levelName:"";
  levelAuthor = currentLevel?currentLevel->m_creatorName:"";

  log::debug(
    "Playing \"{}\" by {} (id: {}), used in \"{}\" by {}", 
    songName, songAuthor, track, levelName, levelAuthor
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
  ended = true;
  log::debug("[AudioManager]: song ended, dispatching music ended event");
  Game::get()->dispatchEvent(new MusicEnded());
}

void AudioManager::set(gd::string filePath, float fadeTime) {
  unsigned int prevFadePoint = 0;
  auto playNewSound = [this,filePath,fadeTime,&prevFadePoint]{
    if (sound) sound->release();
    channel->setChannelGroup(nullptr);
    channel->stop();
    sys->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
    sys->playSound(sound,nullptr,true,&channel);
    seek(0);
    channel->setCallback(&fmodSoundCallback);
    channel->setChannelGroup(FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel);
    ended = false;
    if (fadeTime>0) {
      // fading
      unsigned int* pos;
      // remove fadePoints set by the function below this
      channel->removeFadePoints(prevFadePoint,prevFadePoint+fadeTime*1000);
      // seek forward current+fadeTime to
      channel->setPosition(fadeTime*1000,FMOD_TIMEUNIT_MS);
      // get the fade end position
      channel->getPosition(pos,FMOD_TIMEUNIT_PCM);
      // seek it back
      channel->setPosition(0,FMOD_TIMEUNIT_MS);
      // add fade points
      channel->addFadePoint(0, 0);
      channel->addFadePoint((unsigned long long)pos, 1);
    }
    channel->setPaused(paused);
    Game::get()->dispatchEvent(new MusicStarted());
  };
  if (sound && fadeTime>0 && !ended) {
    runAction(cocos2d::CCSequence::create(
      CCCallFuncL::create([this,fadeTime,&prevFadePoint]{
        unsigned int pos = 0;
        unsigned int posMS = 0;
        // get the current position
        channel->getPosition(&posMS,FMOD_TIMEUNIT_MS);
        prevFadePoint = posMS;
        // do the exact same as what playNewSound did, except with the current position
        channel->setPosition(posMS+fadeTime*1000,FMOD_TIMEUNIT_MS);
        channel->getPosition(&pos,FMOD_TIMEUNIT_PCM);
        channel->setPosition(posMS,FMOD_TIMEUNIT_MS);
        channel->addFadePoint((unsigned long long)posMS, 1);
        channel->addFadePoint((unsigned long long)pos, 0);
      }),
      CCDelayTime::create(fadeTime),
      CCCallFuncL::create(playNewSound),
      nullptr
    ));
  } else {playNewSound();}
}