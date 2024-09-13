#include "OsuGame.hpp"
#include "../helpers/CustomActions.hpp"
#include "overlays/toolbar/ToolbarConstants.hpp"
#include "overlays/SettingsPanel.hpp"

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
    addListener("nodeLayoutUpdate",[this](NodeEvent* e){
        screensContainer->setContentSize({getContentWidth(),getContentHeight()-offset});
        overlaysContainer->setContentSize({getContentWidth(),getContentHeight()-offset});
    });
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

    // j
    scheduleUpdate();
    return true;

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
void OsuGame::pushScreen(Screen* s) {
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
    /*
    if (event->eventName() == "mouseEvent") {
        toolbar->dispatchEvent(event);
        if (currentScreen) currentScreen->dispatchEvent(event);
        return;
    }
    */
    // messy way to get popupdialogs
    if (auto c = getChildren()) {
        c->reverseObjects();
        for (auto* i : CCArrayExt<CCNode*>(c)) {
            if (static_cast<CCBool*>(i->getUserObject("popupdialog"_spr))) {
                // EventTarget is not a cocos2d object
                dynamic_cast<EventTarget*>(i)->dispatchEvent(event);
            }
        }
        c->reverseObjects();
    }
    //EventTarget::dispatchEvent(event);
    toolbar->dispatchEvent(event);
    if (current != nullptr) {
        current->dispatchEvent(event);
    }
    return true;
}

void OsuGame::updateTitle() {
    auto currentScreen = dynamic_cast<Screen*>(current);
    if (currentScreen) {
        auto title = currentScreen->title() + " | osu!lazer";
        
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
    void platformShutdown() {
        log::info("[hook: AppDelegate]: shutdown app");
        OsuGame::get()->release();
        AppDelegate::platformShutdown();
    }
};
#endif
