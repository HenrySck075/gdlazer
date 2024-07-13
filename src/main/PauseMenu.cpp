#include "PauseMenu.hpp"


bool osuPauseLayer::init() {
  // m
    if (!PauseLayer::init(true)) {
      return false;
    }

    CCObject* obj;
    CCARRAY_FOREACH(this->getChildren(), obj) {
        auto n = static_cast<CCNodeRGBA*>(obj);
        int oldOpacity = n->getOpacity();
        n->setOpacity(0);
        n->runAction(CCFadeTo::create(0.2, oldOpacity));

    }

    return true;
}
