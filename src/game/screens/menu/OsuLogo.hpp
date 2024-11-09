#pragma once

/* 
#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../graphics/containers/beatsync/BeatEvent.hpp"
*/
#include "../../../framework/graphics/containers/ClickableContainer.hpp"
#include "../../../framework/audio/AudioManager.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class OsuLogo : public ClickableContainer {
private:
  AudioManager* audio;
  CCSprite* logoSprite;
  int beatCount = 0; // TODO: its not guaranteed to be always 0, but we mute the music anyways so
public:  
  void onMouseEnter() override {};
  void onMouseExit() override {};
  //void onBeat(float delta) override;
  void onMouseUp(MouseEvent* event) override {};
  void onMouseDown(MouseEvent* event) override {};
  void onClick(MouseEvent* e) override {};
  void update(float delta) override;

  static OsuLogo* create() {
    OsuLogo *ret = new OsuLogo();
    if (ret && ret->init()) {
      ret->ClickableContainer::autorelease();
    } else {
      if (ret) {
        delete (ret);
      }
    }
    return ret;
  }
  bool init();
  friend class LogoVisualization;
};

class GEODE_HIDDEN LogoVisualization : public Container {
  CCDrawNode* drawNode;
  float bars[200] = {0.f};
public:
  $default_create(LogoVisualization);
  void update(float delta) override;
  bool init();
};