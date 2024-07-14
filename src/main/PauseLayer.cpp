#include "PauseLayer.hpp"
#include "Geode/binding/PauseLayer.hpp"

#include <Geode/modify/PauseLayer.hpp>
class $modify(PauseLayer) {
    bool init(bool idk) {
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

        FMODAudioEngine::sharedEngine()->playMusic("pause-loop.mp3"_spr, true, 0.25, 0);

        return true;

    }
};
