#pragma once

#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "../../graphics/containers/beatsync/BeatEvent.hpp"
#include "../../graphics/ui/ButtonBase.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class OsuLogo : public ButtonBase {
private:
  BeatDetector* instance;
  EventListenerProtocol* m_listener;
  int beatCount = 0; // TODO: its not guaranteed to be always 0, but we mute the music anyways so
public:  
    void onMouseEnter() override {};
    void onMouseExit() override {};
    MiniFunction<void(float)> onBeat();
  void update(float delta);
  static OsuLogo* create() {
    OsuLogo *ret = new OsuLogo();
    if (ret && ret->init()) {
      ret->ButtonBase::autorelease();
    } else {
      if (ret) {
        delete (ret);
      }
    }
    return ret;
  }
  bool init();
};
