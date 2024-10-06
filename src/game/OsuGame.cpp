#include "OsuGame.hpp"
#include "../helpers/CustomActions.hpp"
#include "overlays/toolbar/ToolbarConstants.hpp"
#include "overlays/SettingsPanel.hpp"
#include "overlays/MusicController.hpp"

#include "../framework/graphics/containers/OverlayContainer.hpp"
#ifdef GEODE_IS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

LONG_PTR oWindowProc;
bool newWindowProcSet = false;

LRESULT CALLBACK nWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    return CallWindowProc((WNDPROC)oWindowProc, hwnd, msg, wparam, lparam);
}

HWND getWindowHandle() {
    return WindowFromDC(wglGetCurrentDC());
}
#endif

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

bool OsuGame::init() {
    CCScene::init();
    screensContainer = Container::create();
    screensContainer->setID("screens");
    screensContainer->setContentSize(getContentSize());
    this->addChild(screensContainer);

    overlaysContainer = Container::create();
    overlaysContainer->setID("overlays");
    overlaysContainer->setContentSize(getContentSize());
    this->addChild(overlaysContainer);

    toolbar = Toolbar::create();
    this->addChild(toolbar);
#ifdef GEODE_IS_WINDOWS
    if (!newWindowProcSet) {
        oWindowProc = SetWindowLongPtrA(getWindowHandle(), -4, (LONG_PTR)nWindowProc);
        newWindowProcSet = true;
    }
#endif
    // making m_pChildren non-nullptr
    overlaysContainer->addChild(CCNode::create());
    overlaysContainer->removeAllChildren();

    overlaysContainer->setCascadeOpacityEnabled(false);
    screensContainer->setCascadeOpacityEnabled(false);

    auto curSize = getContentSize();
    screensContainer->setContentSize(curSize);
    overlaysContainer->setContentSize(curSize);

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

    // j
    scheduleUpdate();
    return true;
}
void OsuGame::startMusicSequence() {
    playlistIndex = -1;
    nextMusic();
    addListener(MusicEnded::eventname, std::bind(&OsuGame::nextMusic,this));
};
void OsuGame::nextMusic() {
    playlistIndex++;
    MusicController::get()->playFromLevel(mainPlaylist[playlistIndex],0);
}
void OsuGame::showToolbar() {
    toolbar->show();
    offset = screensContainer->processUnit(ToolbarConstants::HEIGHT,Unit::UIKit,false);
    screensContainer->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight()-offset)
    ));
    overlaysContainer->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight()-offset)
    ));
}

void OsuGame::hideToolbar() {
    toolbar->hide();
    offset = 0;
    screensContainer->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight())
    ));
    overlaysContainer->runAction(CCEaseOutQuint::create(
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

/*
template<typename T>
concept is_screen = std::is_class_v<Screen>;

template<typename T>
concept is_overlay = std::is_class_v<OverlayContainer>;
*/
Screen* OsuGame::pushScreen(Screen* s) {
    Screen* ls = nullptr;
    if (screenStack.size()!=0) {
        ls = screenStack[screenStack.size()-1];
    }
    auto e = ScreenTransitionEvent(ls,s);
    if (ls) ls->onExiting(e);
    screensContainer->addChild(s);
    s->onEntering(e);
    screenStack.push_back(s);
    current = s;

    updateTitle();

    return s;
}
Screen* OsuGame::replaceScreen(Screen* s) {
    Screen* ls = nullptr;
    if (screenStack.size()!=0) {
        ls = screenStack.pop_back();
    }
    auto e = ScreenTransitionEvent(ls,s);
    if (ls) ls->onExiting(e);
    screensContainer->addChild(s);
    s->onEntering(e);
    screenStack.push_back(s);
    current = s;
    screenPopQueue.push_back(ls);

    updateTitle();

    return s;
}
void OsuGame::pushOverlay(OverlayContainer* o) {
    overlaysContainer->addChild(o);
    overlayStack.push_back(o);
    o->onOpen();
    current = o;
}
OverlayContainer* OsuGame::popOverlay() {return popManyOverlays(1);}
OverlayContainer* OsuGame::popManyOverlays(int amount) {
    if (overlayStack.size()==0) {
        return nullptr;
    }
    OverlayContainer* cs = overlayStack[overlayStack.size()-1];
    OverlayContainer* s = nullptr;
    for (;amount>0;amount--) {
        if (overlayStack.size()!=0) {
            auto s = overlayStack.pop_back();
            s->onClose();
            if (m_pActionManager->numberOfRunningActionsInTarget(s)!=0) overlayPopQueue.push_back(s);
            else overlaysContainer->removeChild(s);
        }
        else break;
    }
    OverlayContainer* ps = nullptr;
    if (overlayStack.size()!=0) ps = overlayStack[overlayStack.size()-1];
    
    if (ps) current = ps;
    else current = *(screenStack.begin()+screenStack.size()-1);

    return s;
}

Screen* OsuGame::popManyScreens(int amount, bool popOverlays) {
    if (screenStack.size()==0) {
        return nullptr;
    }
    if (popOverlays) popManyOverlays(overlaysContainer->getChildrenCount());
    bool schedulePause = screenStack.size()==1;
    Screen* cs = screenStack[screenStack.size()-1];
    Screen* s = nullptr;
    for (;amount>0;amount--) {
        if (screenStack.size()!=0) {
            auto s = screenStack.pop_back();
            screenPopQueue.push_back(s);
        }
        else break;
    }
    Screen* ps = nullptr;
    if (screenStack.size()!=0) ps = screenStack[screenStack.size()-1];
    
    ScreenTransitionEvent event = {cs,ps};
    for (auto* i : screenPopQueue) {}
    if (ps!=nullptr) ps->onEntering(event);
    current = ps;
    if (schedulePause) m_pActionManager->pauseTarget(this);

    updateTitle();
    
    return s;
}

Screen* OsuGame::popScreen(bool popOverlays) {return popManyScreens(1,popOverlays);}

template<typename T>
T* OsuGame::popUntilScreenType() {
    int idx = 0;
    for (Screen* s : screenStack) {
        if (dynamic_cast<T*>(s)) return popManyScreens(screenStack.size()-idx);
        idx++;
    }
    return nullptr; // nada
};

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

    auto i2 = overlayPopQueue.inner();
    for (auto* o : overlayPopQueue) {
        if (m_pActionManager->numberOfRunningActionsInTarget(o) == 0) {
            overlaysContainer->removeChild(o);
            i2->removeObject(o);
        }
    };
}

bool OsuGame::dispatchEvent(NodeEvent* event) {
    if (event->target() != nullptr) return false;
    if (!this->isRunning()) return true;
    if (event->eventName().starts_with("og")) {
        EventTarget::dispatchEvent(event);
        return true;
    }
    updateDispatchFlow(event, DispatchingFlow::Down);

    toolbar->dispatchEvent(event);
    if (event->eventName() != "nodeLayoutUpdate") {
        if (current != nullptr) {
            current->dispatchEvent(event);
        }
    } else {
        screensContainer->setContentSize({getContentWidth(),getContentHeight()-offset});
        overlaysContainer->setContentSize({getContentWidth(),getContentHeight()-offset});
    }
    return true;
}

void OsuGame::updateTitle() {
    auto currentScreen = dynamic_cast<Screen*>(current);
    if (currentScreen) {
        auto title = fmt::format("{}{}osu!lazer", currentScreen->title(), currentScreen->title().empty()?"":" | ");
        
        #ifdef GEODE_IS_WINDOWS

        SetWindowTextA(getWindowHandle(), title.c_str());

        #endif
    }
}

void OsuGame::update(float dt) {
    checkForQueue();
    e();
    if (volMultChange) {
        volMult -= dt;
        auto engine = FMODAudioEngine::sharedEngine();
        engine->setBackgroundMusicVolume(bgVol*volMult);
        engine->setEffectsVolume(sfxVol*volMult);
        if (volMult <= 0.6) {
            volMultChange = false;
            volMult = 1;
        }
    }
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
        AppDelegate::trySaveGame(p0);
    }
};
#endif
