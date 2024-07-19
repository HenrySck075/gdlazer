#pragma once

#include "../../graphics/containers/BeatSyncedContainer.hpp"
#include "../../graphics/ui/ButtonBase.hpp"
#include <Geode/cocos/include/cocos2d.h>
using namespace cocos2d;

class OsuLogo : public BeatSyncedContainer, public ButtonBase {
private:
  void onClick(CCNode* self);
public:  
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
