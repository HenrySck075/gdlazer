#pragma once

#include <regex>
#include <Geode/Geode.hpp>

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:
	
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
	
	std::string path = "";

	static Manager* getSharedInstance(){

		if (!instance) {
			instance = new Manager();
		};
		return instance;
	}

  void playMusic(gd::string path, bool loop) {
    system->createSound(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &(sound));
    sound->setLoopCount(loop?-1:0);
    system->playSound(sound, nullptr, false, &(channel));
    channel->setVolume(100);
  }

};
