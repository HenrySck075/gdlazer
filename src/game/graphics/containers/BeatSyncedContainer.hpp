#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include "beatsync/BeatEvent.hpp"

// Implementation class, not a real container (because if mixed with literally anything else this will make the making experience sucks)
//
// Note: one must make the create function themselves
class BeatSyncedContainer {
private:
	EventListener<EventFilter<BeatEvent>> m_listener = { [this](BeatEvent* n) {
			log::info("beat");
			this->onBeat(n->m_elapsed);
			return ListenerResult::Propagate;
		} };
public:
	virtual void onBeat(float elapsed) = 0;
};
