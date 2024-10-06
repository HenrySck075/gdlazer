#include "MusicController.hpp"
#include <Geode/cocos/actions/CCActionManager.h>
#include "../../helpers/CustomActions.hpp"
#include "../OsuGame.hpp"

static MusicController* instance;
MusicController* MusicController::get() {
    if (!instance) {
        instance = new MusicController();
        if (!instance->init()) delete instance;
    }
    return instance;
};

bool MusicController::init() {
    sys = FMODAudioEngine::sharedEngine()->m_system;
    FMOD::ChannelGroup* masterChannel = FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel;
    tools = new LevelTools();
    
    sys->createDSPByType(FMOD_DSP_TYPE_FFT, &dsp);
    masterChannel->addDSP(7, dsp);

    return true;
}
void MusicController::playFromLevel(GJGameLevel* level, float fadeTime) {
    currentLevel = level;
    log::debug("[MusicController]: {}", tools->getAudioFileName(level->m_audioTrack).c_str());

    songName = currentLevel
        ? tools->getAudioTitle(currentLevel->m_audioTrack)
        : "The Most Mysterious Song On The Internet";
    songAuthor = currentLevel
        ? tools->nameForArtist(tools->artistForAudio(currentLevel->m_audioTrack))
        : "Unknown";
    levelName = currentLevel?currentLevel->m_levelName:"";
    levelAuthor = currentLevel?currentLevel->m_creatorName:"";

    set(
        CCFileUtils::sharedFileUtils()->fullPathForFilename(
            tools->getAudioFileName(level->m_audioTrack).c_str(),false
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
        MusicController* ctrl;
        reinterpret_cast<FMOD::Channel*>(channel)->getUserData((void**)&ctrl);
        ctrl->onSongEnd();
    }
    return FMOD_RESULT::FMOD_OK;
};

void MusicController::onSongEnd() {
    paused = true;
    OsuGame::get()->dispatchEvent(new MusicEnded());
}

void MusicController::set(gd::string filePath, float fadeTime) {
    unsigned int prevFadePoint = 0;
    auto playNewSound = [this,filePath,fadeTime,&prevFadePoint]{
        if (sound) sound->release();
        bool channelNotCreated = !channel;
        sys->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
        sys->playSound(sound,nullptr,true,&channel);
        if (channelNotCreated) {
            channel->setUserData(this);
            channel->setCallback(&fmodSoundCallback);
            channel->setChannelGroup(FMODAudioEngine::sharedEngine()->m_backgroundMusicChannel);
        }
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
    };
    if (sound && fadeTime>0) {
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