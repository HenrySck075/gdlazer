#pragma once

/* 
#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../graphics/containers/beatsync/BeatEvent.hpp"
*/
#include "../../../frameworks/graphics/containers/ClickableContainer.hpp"
#include "../../../frameworks/audio/AudioManager.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

GDL_NS_START
class OsuLogo : public frameworks::ClickableContainer {
private:
  Ref<frameworks::AudioManager> audio;
  Ref<CCSprite> logoSprite;
  //int beatCount = 0; // TODO: its not guaranteed to be always 0, but we mute the music anyways so
public: 
  void onEnter() override {
    CCNode::onEnter();
    scheduleUpdate();
  }
  void update(float delta) override;

  static OsuLogo* create() {
    $create_class(OsuLogo, init);
  }
  bool init();
  friend class LogoVisualization;
};

class GEODE_HIDDEN LogoVisualization : public frameworks::Container {
  Ref<CCDrawNode> drawNode;
  float bars[200] = {0.f};
public:
  $default_create(LogoVisualization);
  void update(float delta) override;
  bool init();
};
GDL_NS_END