#include "Game.hpp"
#include "../helpers/CustomActions.hpp"

#include "graphics/containers/OverlayContainer.hpp"
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

extern char const returnEventsToScreen[] = "returnEventsToScreen";

using ReturnFromOverlay = NamedNodeEvent<returnEventsToScreen>;

/// we assume shit in here
class OverlaysWatcherContainer : public Container {
    CCArrayExt<OverlayContainer*> overlayPopQueue;
    CCArrayExt<OverlayContainer*> overlayStack;
public:
    default_create(OverlaysWatcherContainer);

    bool init() {
        if (!Container::init()) return false;
        setColor({0,0,0,127});
        setOpacity(0);
        scheduleUpdate();
        return true;
    }

    void update(float dt) override {
        for (OverlayContainer* i : overlayPopQueue) {
            if (m_pActionManager->numberOfRunningActionsInTarget(i) == 0) {
                CCNode::removeChild(i);
            }
        }
    }

    void addChild(CCNode* child) {
        if (m_pChildren->count()==0) runAction(CCFadeIn::create(0.25));
        Container::addChild(child);
    }
    void removeChild(CCNode* child) {
        if (m_pChildren->count()==1) {
            runAction(CCFadeOut::create(0.25));
            static_cast<Game*>(m_pParent)->dispatchEvent(new ReturnFromOverlay());
        }
        auto c = static_cast<OverlayContainer*>(child);
        if (m_pActionManager->numberOfRunningActionsInTarget(c) != 0) overlayPopQueue.push_back(c);
    }
};

Game* Game::instance = nullptr;

/// TODO: Move overlays opening/closing logic to (Osu)OverlayContainer
bool Game::init() {
    CCScene::init();

    // The "Geode way" of doing things is that 

    // first someone makes a feature, but then realizes it had a really weird quirk 
    // so they had to make a really weird design for it that seems utterly stupid at first glance 
    // and so every other lead dev ridicules them 

    // but then when they try to "fix" it they eventually stumble upon the exact same solution
    main = Container::create();
    main->setContentSize(getContentSize());

    screensContainer = Container::create();
    screensContainer->setID("screens");
    screensContainer->setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
    main->addChild(screensContainer);

    overlaysContainer = OverlaysWatcherContainer::create();
    overlaysContainer->setID("overlays");
    overlaysContainer->setContentSizeWithUnit({100,100},Unit::Percent,Unit::Percent);
    main->addChild(overlaysContainer);

#ifdef GEODE_IS_WINDOWS
    if (!newWindowProcSet) {
        oWindowProc = SetWindowLongPtrA(getWindowHandle(), -4, (LONG_PTR)nWindowProc);
        newWindowProcSet = true;
    }
#endif
    // making m_pChildren non-nullptr
    overlaysContainer->addChild(CCNode::create());
    overlaysContainer->removeAllChildren();
    overlaysContainer->setColor({0,0,0,127});

    overlaysContainer->setCascadeOpacityEnabled(false);
    screensContainer->setCascadeOpacityEnabled(false);

    auto curSize = getContentSize();
    screensContainer->setContentSize(curSize);
    overlaysContainer->setContentSize(curSize);

    // j
    scheduleUpdate();

    addListener(returnEventsToScreen, [this](NodeEvent* e){
        current = *(screenStack.begin()+screenStack.size()-1);
    });
    return true;
}

/*
template<typename T>
concept is_screen = std::is_class_v<Screen>;

template<typename T>
concept is_overlay = std::is_class_v<OverlayContainer>;
*/
Screen* Game::pushScreen(Screen* s) {
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
Screen* Game::replaceScreen(Screen* s) {
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
void Game::pushOverlay(OverlayContainer* o) {
    // call show if the overlay isn't open
    if (o->isOpen()) {
        o->show();
        return;
    }
    overlaysContainer->addChild(o);
    current = overlaysContainer;
}
OverlayContainer* Game::popOverlay(OverlayContainer* overlay) {
    overlaysContainer->removeChild(overlay);

    return overlay;
}

Screen* Game::popManyScreens(int amount, bool popOverlays) {
    if (screenStack.size()==0) {
        return nullptr;
    }
    if (popOverlays); //popManyOverlays(overlaysContainer->getChildrenCount());
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

Screen* Game::popScreen(bool popOverlays) {return popManyScreens(1,popOverlays);}

template<typename T>
T* Game::popUntilScreenType() {
    int idx = 0;
    for (Screen* s : screenStack) {
        if (dynamic_cast<T*>(s)) return popManyScreens(screenStack.size()-idx);
        idx++;
    }
    return nullptr; // nada
};

void Game::onLoseFocus() {
    auto engine = FMODAudioEngine::sharedEngine();
    bgVol = engine->getBackgroundMusicVolume();
    sfxVol = engine->getEffectsVolume();
    volMultChange = true;
}
void Game::onFocus() {
    volMultChange = false;
    auto engine = FMODAudioEngine::sharedEngine();
    engine->setBackgroundMusicVolume(bgVol);
    engine->setEffectsVolume(bgVol);
}

void Game::checkForQueue() {
    auto i = screenPopQueue.inner();
    for (auto* s : screenPopQueue) {
        if (m_pActionManager->numberOfRunningActionsInTarget(s) == 0) {
            screensContainer->removeChild(s);
            i->removeObject(s);
        }
    };
}

bool Game::dispatchEvent(NodeEvent* event) {
    if (event->target() != nullptr) return false;
    if (!this->isRunning()) return true;
    if (event->eventName().starts_with("og")) {
        EventTarget::dispatchEvent(event);
        return true;
    }
    updateDispatchFlow(event, DispatchingFlow::Down);

    if (event->eventName() != "nodeLayoutUpdate") {
        if (current != nullptr) {
            current->dispatchEvent(event);
        }
    } else {
        screensContainer->setContentSize({getContentWidth(),getContentHeight()-offset});
        main->setContentSize({getContentWidth(),getContentHeight()-offset});
    }
    return true;
}

void Game::updateTitle() {
    auto currentScreen = dynamic_cast<Screen*>(current);
    if (currentScreen) {
        auto title = fmt::format("{}{}osu!lazer", currentScreen->title(), currentScreen->title().empty()?"":" | ");
        
        #ifdef GEODE_IS_WINDOWS

        SetWindowTextA(getWindowHandle(), title.c_str());

        #endif
    }
}

void Game::update(float dt) {
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
        Game::get()->onFocus();
    }
    void applicationWillResignActive() {
        AppDelegate::applicationWillResignActive();
        Game::get()->onLoseFocus();
    }
    void trySaveGame(bool p0) {
        log::info("[hook: AppDelegate]: shutdown app");
        Game::get()->release();
        AppDelegate::trySaveGame(p0);
    }
};
#endif