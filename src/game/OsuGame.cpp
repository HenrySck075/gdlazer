#include "OsuGame.hpp"
#include "../helpers/CustomActions.hpp"
#include "overlays/toolbar/ToolbarConstants.hpp"
#include "overlays/SettingsPanel.hpp"
#include "../framework/audio/AudioManager.hpp"

#include "../framework/graphics/containers/OverlayContainer.hpp"

#include <Geode/binding/GJGameLevel.hpp>

#include <Geode/modify/LevelTools.hpp>
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
            case -7: return "triangles.mp3"_spr;
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

    toolbar = Toolbar::create();
    this->addChild(toolbar);

    addListener("mouseEvent", [this](NodeEvent* e){toolbar->dispatchEvent(e);});

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
            mainPlaylist.push_back(level);
            addedSong.push_back(level->m_audioTrack);
            levelDownloadCount.push_back(level->m_downloads);
        }
    }

    return true;
}
void OsuGame::startMusicSequence() {
    playlistIndex = -1;
    nextMusic();
    addListener(MusicEnded::eventname, std::bind(&OsuGame::nextMusic,this));
};
void OsuGame::nextMusic() {
    playlistIndex++;
    AudioManager::get()->playFromLevel(mainPlaylist[playlistIndex],0);
}
void OsuGame::showToolbar() {
    toolbar->show();
    offset = main->processUnit(ToolbarConstants::HEIGHT,Unit::UIKit,false);
    main->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight()-offset)
    ));
}

void OsuGame::hideToolbar() {
    toolbar->hide();
    offset = 0;
    main->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight())
    ));
}

void OsuGame::showSettings() {
    pushOverlay(overlays["settings"]);
}
void OsuGame::hideSettings() {
    // i hope so
    popOverlay();
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

#ifndef GEODE_IS_ANDROID
#include <Geode/modify/AppDelegate.hpp>
class $modify(AppDelegate) {
    void applicationWillBecomeActive() {
        AppDelegate::applicationWillBecomeActive();
        OsuGame::get()->onFocus();
    }
    void applicationWillResignActive() {
        AppDelegate::applicationWillResignActive();
        OsuGame::get()->onLoseFocus();
    }
    void trySaveGame(bool p0) {
        log::info("[hook: AppDelegate]: shutdown app");
        OsuGame::get()->release();
        do {
            GameManager::sharedState()->m_menuLayer->release();
        // lel
        } while (GameManager::sharedState()->m_menuLayer && GameManager::sharedState()->m_menuLayer->retainCount()>0);
        AppDelegate::trySaveGame(p0);
    }
};
#endif
