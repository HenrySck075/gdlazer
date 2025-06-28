#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>
#include <type_traits>
#include "bindables/EventTarget.hpp"
#include "graphics/containers/OverlayContainer.hpp"
#include "screens/Screen.hpp"


GDF_NS_START

class Game : public cocos2d::CCScene, public EventTarget {
protected:
  bool doDispatchEvent(Event* event, std::type_index type) override;

  geode::Ref<Container> m_everypence;

  friend class DebugDrawNode;
  geode::cocos::CCArrayExt<Screen> m_screenStack;
  // techinically we can do m_screenStack->lastObject() but
  // since we're also spamming MouseEvent every frame doing this is faster
  geode::Ref<Screen> m_currentScreen;
  geode::Ref<Container> m_screensContainer;
  
  geode::cocos::CCArrayExt<OverlayContainer> m_overlayStack;
  geode::Ref<OverlayContainer> m_currentOverlay;
  geode::Ref<Container> m_overlaysContainer;

  geode::cocos::CCArrayExt<Screen> m_invisibleQueue;
  geode::cocos::CCArrayExt<Screen> m_removalQueue;
  geode::cocos::CCArrayExt<OverlayContainer> m_overlayRemovalQueue;

  static void setInstance(geode::Ref<Game> instance);
  // lame
  virtual bool doDEMidhook(Event* event, std::type_index type) {};

  virtual void setWindowTitle(std::string title);
public:
  inline float processUnit(float value, Unit unit, bool isWidth) {
    return m_everypence->processUnit(value, unit, isWidth);
  }
  virtual bool init() override;
  void yeah(cocos2d::CCObject*) {
    cocos2d::CCDirector::get()->popScene();
  }
  void weh(cocos2d::CCObject*) {
    MenuLayer::get()->onMoreGames(nullptr);
  }

  void onExit() override {}
  void onExitTransitionDidStart() override {}
  void cleanup() override {}
  void onEnter() override {
    if (!m_bRunning) CCNode::onEnter();
  }

  void pushScreen(Screen* screen);
  Screen* popScreen();
  void replaceScreen(Screen* screen);

  /// Use `OverlayContainer::show()` instead idiots
  void pushOverlay(OverlayContainer* overlay);
  void popOverlay(OverlayContainer* overlay);

  void update(float dt);

  static geode::Ref<Game> get(bool createIfNotExist = true);
};
GDF_NS_END
