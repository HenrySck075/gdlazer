#include "OsuGame.hpp"
#include "../helpers/CustomActions.hpp"
#include "overlays/toolbar/ToolbarConstants.hpp"
#include "overlays/SettingsPanel.hpp"
#include "../framework/audio/AudioManager.hpp"
#include <henrysck075.easings/include/easings.hpp>

#include "../framework/graphics/containers/OverlayContainer.hpp"
#include "../framework/input/events/KeyEvent.hpp"

#include <Geode/binding/GJGameLevel.hpp>

#include <Geode/modify/LevelTools.hpp>
#include <random>
struct LevelToolsCustomSong : geode::Modify<LevelToolsCustomSong, LevelTools> {
  static gd::string getAudioTitle(int trackID) {
    switch (trackID) {
      case -7: return "Triangles";
      case -8: return "circles!";
      default: return LevelTools::getAudioTitle(trackID);
    }
  }
  static int artistForAudio(int trackID) {
    switch (trackID) {
      case -7: return -1;
      case -8: return -2;
      default: return LevelTools::artistForAudio(trackID);
    }
  }
  static gd::string getAudioFileName(int trackID) {
    switch (trackID) {
      case -7: return "triangle.mp3"_spr;
      case -8: return "StereoMadness.mp3";
      default: return LevelTools::getAudioFileName(trackID);
    }
  }
  static gd::string nameForArtist(int artistID) {
    switch (artistID) {
      case -1: return "cYsmix";
      case -2: return "nekodex";
      default: return LevelTools::nameForArtist(artistID);
    }
  }
};

OsuGame* OsuGame::instance = nullptr;

/// TODO: Move overlays opening/closing logic to (Osu)OverlayContainer
bool OsuGame::init() {
  Game::init();

  // The "Geode way" of doing things is that 

  // first someone makes a feature, but then realizes it had a really weird quirk 
  // so they had to make a really weird design for it that seems utterly stupid at first glance 
  // and so every other lead dev ridicules them 

  // but then when they try to "fix" it they eventually stumble upon the exact same solution

  addListener("keyboardEvent", [this](NodeEvent* e){
    KeyInfo& k = static_cast<KeyboardEvent*>(e)->key;
    if (k.key == KEY_R && k.ctrl && k.shift) {
      game::restart();
    }
  });

  toolbar = Toolbar::create();
  addChild(toolbar);
  //GameManager::get()->m_menuLayer->onMoreGames(nullptr);

  // preload overlays
  overlays["settings"] = SettingsPanel::create();

  // lag the game by adding every 1000+ downloads saved levels to the playlist
  do {
    auto triangles = GJGameLevel::create();
    triangles->m_levelName = "Triangles";
    triangles->m_creatorName = "gd!lazer";
    triangles->m_audioTrack = -7;
    mainPlaylist.push_back(triangles);
  } while (0);

  auto onlineLevels = GameLevelManager::sharedState()->m_onlineLevels;
  // 1st: song id
  std::vector<int> addedSong;
  std::vector<int> levelDownloadCount;
  CCDictElement* e;
  CCDICT_FOREACH(onlineLevels, e) {
    auto level = static_cast<GJGameLevel*>(e->getObject());
    /*
    decltype(addedSong)::iterator pos = std::find(addedSong.begin(),addedSong.end(),level->m_audioTrack);
    if (pos!=addedSong.end()) {
      // replace if the stored level download count is lower than the current one
      int idx = pos-addedSong.begin();
      if (level->m_downloads >= 1000 && levelDownloadCount[idx]<level->m_downloads) {
        addedSong.erase(pos);
        addedSong.insert(pos, level->m_audioTrack);
        levelDownloadCount.erase(levelDownloadCount.begin()+idx);
        levelDownloadCount.insert(levelDownloadCount.begin()+idx, level->m_downloads);  
        mainPlaylist.inner()->replaceObjectAtIndex(idx+1, level);
      }
    }
    else {
    */
      mainPlaylist.push_back(level);
      if (level->m_songIDs.empty()) {
        addedSong.push_back(level->m_audioTrack);
      } else {
        
      }
      levelDownloadCount.push_back(level->m_downloads);
    //}
  }

  return true;
}
void OsuGame::startMusicSequence() {
  playlistIndex = -1;
  nextMusic();
  addListener("musicEnded", std::bind(&OsuGame::nextMusic,this));
};
// currently only supports sequenced, shuffle soon idk
void OsuGame::nextMusic() {
  playlistIndex++;
  AudioManager::get()->playFromLevel(mainPlaylist[playlistIndex],0);
}
void OsuGame::showToolbar() {
  toolbar->show();
  offset = main->processUnit(ToolbarConstants::HEIGHT,Unit::UIKit,false);
  auto s = CCNode::getContentSize();
  main->runAction(easingsActions::CCEaseOut::create(
    CCResizeTo::create(0.5,s.width,s.height-offset),5
  ));
}

void OsuGame::hideToolbar() {
  toolbar->hide();
  offset = 0;
  auto s = CCNode::getContentSize();
  main->runAction(easingsActions::CCEaseOut::create(
    CCResizeTo::create(0.5,s.width,s.height),5
  ));
}

void OsuGame::showSettings() {
  overlays["settings"]->show();
}
void OsuGame::hideSettings() {
  overlays["settings"]->hide();
}
static std::array<std::string, 4> uiRelatedEvents = {
  "nodeLayoutUpdate",
  "mouseEvent",
  "keyboardEvent",
  "keypadEvent"
};
bool OsuGame::dispatchEvent(NodeEvent* event) {
  if (event->target() != nullptr) return false;
  if (!isRunning()) return true;
  EventTarget::dispatchEvent(event);
  updateDispatchFlow(event, DispatchingFlow::Down);
  toolbar->dispatchEvent(event);

  if (event->m_cancelled) return false;

  // direct ui-related events to the current screen
  auto en = event->eventName();
  if (
    en == "nodeLayoutUpdate" ||
    en == "mouseEvent" ||
    en == "keyboardEvent" ||
    en == "keypadEvent"
  ){ 
    if (current != nullptr && event->eventName() != "nodeLayoutUpdate") {
      return current->dispatchEvent(event);
    } else {
      main->setContentSize({getContentWidth(),getContentHeight()-offset});
      return true;
    }
  // for everything else
  } else {
    return main->dispatchEvent(event);
  }
}

void OsuGame::onLoseFocus() {
  auto engine = FMODAudioEngine::sharedEngine();
  bgVol = engine->getBackgroundMusicVolume();
  sfxVol = engine->getEffectsVolume();
  volMultChange = true;
}
void OsuGame::onFocus() {
  volMultChange = false;
  auto engine = FMODAudioEngine::sharedEngine();
  engine->setBackgroundMusicVolume(bgVol);
  engine->setEffectsVolume(bgVol);
}

void OsuGame::checkForQueue() {
  auto i = screenPopQueue.inner();
  for (auto* s : screenPopQueue) {
    if (m_pActionManager->numberOfRunningActionsInTarget(s) == 0) {
      screensContainer->removeChild(s);
      i->removeObject(s);
    }
  };
}

