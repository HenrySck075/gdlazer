#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include "beatsync/BeatEvent.hpp"

// the j
//
// Note: one must make the create function themselves
class BeatSyncedContainer : public CCNode {
private:
  EventListener<EventFilter<BeatEvent>> m_listener = {&BeatSyncedContainer::onBeatInternal};
  void onBeatInternal(float elapsed) {this->onBeat(elapsed);};
public:
  void onBeat(float elapsed) {};
};
