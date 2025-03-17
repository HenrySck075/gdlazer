#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>
#include "bindables/EventTarget.hpp"
#include "Geode/utils/cocos.hpp"


GDL_NS_START

class Game : public cocos2d::CCScene, public EventTarget {
public:
  bool dispatchEvent(Event *event) override;
  bool init();
  void yeah(cocos2d::CCObject*) {
    cocos2d::CCDirector::get()->popScene();
  }
  static Game* get();
};

GDL_NS_END