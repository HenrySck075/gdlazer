#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>
#include "Geode/binding/MenuLayer.hpp"
#include "bindables/EventTarget.hpp"
#include "Geode/utils/cocos.hpp"
#include "graphics/containers/Container.hpp"


GDF_NS_START

class Game : public cocos2d::CCScene, public EventTarget {
protected:
  bool doDispatchEvent(Event *event, std::type_index type) override;

public:
  bool init();
  void yeah(cocos2d::CCObject*) {
    cocos2d::CCDirector::get()->popScene();
  }
  void weh(cocos2d::CCObject*) {
    MenuLayer::get()->onMoreGames(nullptr);
  }
  static Game* get(bool createIfNotExist = true);
};

GDF_NS_END
