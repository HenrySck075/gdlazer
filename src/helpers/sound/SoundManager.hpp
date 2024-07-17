#pragma once

#include <Geode/Geode.hpp>

// SoundManager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class SoundManager {

protected:
	static SoundManager* instance;
public:
	
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
	
	std::string path = "";

	static SoundManager* sharedManager(){

		if (!instance) {
			instance = new SoundManager();
		};
		return instance;
	}

  void playMusic(gd::string path, bool loop) {
    system->createSound((path).c_str(), FMOD_LOOP_NORMAL, nullptr, &(sound));
		sound->setLoopCount(-1);
		system->playSound((sound), nullptr, false, &(channel));
		channel->setVolume(1);
    channel->addFadePoint(0, 0);
    channel->addFadePoint(1, 1);
  }

};
