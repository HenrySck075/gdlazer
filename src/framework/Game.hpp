#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../utils.hpp"
#include "graphics/containers/Container.hpp"
#include "bindables/EventTarget.hpp"
using namespace cocos2d;

// the
class Screen;
#include "screens/Screen.hpp"
#include "screens/ScreenTransitionEvent.hpp"
#include "graphics/containers/OverlayContainer.hpp"

// the grand dad
//
// funny
class Game : public CCScene, public EventTarget {
protected:
  float offset = 0;

  CCArrayExt<Screen*> screenStack;
  CCArrayExt<Screen*> screenPopQueue;
  Container* screensContainer;
  Container* overlaysContainer;

  Container* main;
  
  float bgVol = 1;
  float sfxVol = 1;
  float volMult = 1;
  bool volMultChange = false;

  /// can be overlay or screen
  Container* current = nullptr;
  void g();

  static Game* createInstance();
protected:
  /// set the current instance
  static void setInstance(Game* instance);
  /// get the current instance
  static Game* getInstance();
public:
  static Game* get();

  void e() {
    auto m = CCDirector::sharedDirector()->getOpenGLView()->getDesignResolutionSize();
    if (m!=getContentSize()) {
      setContentSize(m);
      dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
    }
  }


  bool dispatchEvent(NodeEvent* event) override;

  bool init();

  void showToolbar();
  void hideToolbar();

  void showSettings();
  void hideSettings();
  // push screen and return the pushed screen if one needs to do more stuff with it (does not include removing the screen)
  Screen* pushScreen(Screen* s);
  Screen* replaceScreen(Screen* s);
  Screen* popManyScreens(int amount = 1, bool popOverlays = true);
  Screen* popScreen(bool popOverlays = true);
  void pushOverlay(OverlayContainer* o);
  OverlayContainer* popManyOverlays(int amount = 1);
  OverlayContainer* popOverlay(OverlayContainer* overlay = nullptr);
  template<typename T>
  T* popUntilScreenType();

  void onLoseFocus();
  void onFocus();

  void checkForQueue();

  void setContentSize(CCSize const& size) override {
    CCNode::setContentSize(size);
    dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
  }
  // on android, this does nothing
  void updateTitle();

  void update(float dt);

  friend class OverlaysWatcherContainer;
};

/*
$execute {
  new EventListener<AttributeSetFilter>(
    +[](AttributeSetEvent* event) {
      auto osuGame = Game::sharedScene();
      osuGame->
    },
    AttributeSetFilter()
  );
}
*/
