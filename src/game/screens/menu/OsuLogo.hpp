#pragma once

#include "../../graphics/containers/beatsync/BeatDetector.hpp"
#include "../../graphics/ui/ButtonBase.hpp"
#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

class OsuLogo : public ButtonBase {
private:
  BeatDetector* instance;
  void onClick(CCNode* self);
public:  
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
