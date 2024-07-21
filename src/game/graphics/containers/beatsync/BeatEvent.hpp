#include <Geode/loader/Event.hpp>
#include <Geode/cocos/cocoa/CCObject.h>

class BeatEvent : public geode::Event {
private:
	float m_elapsed;
public:
	BeatEvent(cocos2d::CCObject* the, float elapsed) {
		m_elapsed = elapsed;
	}

	float getElapsed() { return m_elapsed; }
};