#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>
#include <type_traits>
#include "bindables/EventTarget.hpp"
#include "graphics/containers/OverlayContainer.hpp"
#include "screens/Screen.hpp"


GDF_NS_START

template<typename T>
concept GameSubclass = std::is_base_of_v<Game, T>;

class Game : public cocos2d::CCScene, public EventTarget {
protected:
  bool doDispatchEvent(Event *event, std::type_index type) override;

  geode::cocos::CCArrayExt<Screen> m_screenStack;
  // techinically we can do m_screenStack->lastObject() but
  // since we're also spamming MouseEvent every frame doing this is faster
  geode::Ref<Screen> m_currentScreen;
  geode::Ref<Container> m_screensContainer;

  geode::Ref<OverlayContainer> m_currentOverlay;
  geode::Ref<OverlayContainer> m_overlaysContainer;

  geode::cocos::CCArrayExt<Screen> m_invisibleQueue;
  geode::cocos::CCArrayExt<Screen> m_removalQueue;

  static void setInstance(geode::Ref<Game> instance);
public:
  bool init();
  void yeah(cocos2d::CCObject*) {
    cocos2d::CCDirector::get()->popScene();
  }
  void weh(cocos2d::CCObject*) {
    MenuLayer::get()->onMoreGames(nullptr);
  }

  void pushScreen(Screen* screen);
  Screen* popScreen();

  void pushOverlay(OverlayContainer* overlay) {};
  void popOverlay(OverlayContainer* overlay) {};

  void update(float dt);

  static geode::Ref<Game> get(bool createIfNotExist = true);
  template<GameSubclass T>
  static geode::Ref<T> getAs(bool createIfNotExist = true) {
    auto instance = get();
    if ((!instance || dynamic_cast<T*>(instance.operator->())) && createIfNotExist) {
      instance = new T();
      instance->autorelease();
      instance->init();
      setInstance(instance);
    }
    return instance;

  };
};
GDF_NS_END
