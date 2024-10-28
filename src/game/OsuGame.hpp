#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../utils.hpp"
#include "../framework/Game.hpp"
using namespace cocos2d;

// widgets
#include "overlays/toolbar/Toolbar.hpp"

// the
class Screen;

// the grand dad
//
// funny
class OsuGame : public Game {
private:
  float offset = 0;
  static OsuGame* instance;

  Toolbar* toolbar;

  float bgVol = 1;
  float sfxVol = 1;
  float volMult = 1;
  bool volMultChange = false;
  
  /// preloaded overlays
  CCDictionaryExt<std::string, OverlayContainer*> overlays;

  /// list of level songs to play
  CCArrayExt<GJGameLevel*> mainPlaylist;
  int playlistIndex = 0;

  static OsuGame* createInstance() {
    OsuGame* ret = new OsuGame(); 
    if (ret && ret->init()) { ret->autorelease(); } 
    else { 
      do { if (ret) { (ret)->release(); (ret) = 0; } } while (0); 
    }; 
    setInstance(ret);
    return instance;
  }
public:
  static OsuGame* get() {
    auto instance = typeinfo_cast<OsuGame*>(getInstance());
    if (instance == nullptr) return createInstance();
    return instance;
  }

  void startMusicSequence();
  void nextMusic();

  bool init();

  void showToolbar();
  void hideToolbar();

  void showSettings();
  void hideSettings();
  
  void onLoseFocus();
  void onFocus();

  void checkForQueue();

  // on android, this does nothing
  void updateTitle();

};

/*
$execute {
  new EventListener<AttributeSetFilter>(
    +[](AttributeSetEvent* event) {
      auto osuGame = OsuGame::sharedScene();
      osuGame->
    },
    AttributeSetFilter()
  );
}
*/
