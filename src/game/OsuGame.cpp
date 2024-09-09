#include "OsuGame.hpp"
#include "../helpers/CustomActions.hpp"
#include "overlays/toolbar/ToolbarConstants.hpp"

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
    addListener("ogExitDidFinish", [this](NodeEvent* e){
        screensContainer->removeChild(static_cast<ScreenTransitionNotifier*>(e)->caller);
    });
    // making m_pChildren non-nullptr
    overlaysContainer->addChild(CCNode::create());
    overlaysContainer->removeAllChildren();

    auto curSize = getContentSize();
    screensContainer->setContentSize(curSize);
    overlaysContainer->setContentSize(curSize);
    return true;

}

void OsuGame::showToolbar() {
    toolbar->show();
    screensContainer->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight()-screensContainer->processUnit(ToolbarConstants::HEIGHT,Unit::UIKit,false))
    ));
    overlaysContainer->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight()-overlaysContainer->processUnit(ToolbarConstants::HEIGHT,Unit::UIKit,false))
    ));
}

void OsuGame::hideToolbar() {
    toolbar->hide();
    screensContainer->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight())
    ));
    overlaysContainer->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,getContentWidth(),getContentHeight())
    ));
}

/*
template<typename T>
concept is_screen = std::is_class_v<Screen>;

template<typename T>
concept is_overlay = std::is_class_v<OverlayContainer>;
*/
void OsuGame::pushScreen(Container* screen_or_overlay) {
    Screen* ls = nullptr;
    if (auto s = dynamic_cast<Screen*>(screen_or_overlay)) {
        if (screenStack.size()!=0) {
            ls = screenStack[screenStack.size()-1];
        }
        auto e = ScreenTransitionEvent(ls,s);
        if (ls) ls->onExiting(e);
        screensContainer->addChild(s);
        s->onEntering(e);
        screenStack.push_back(s);
        current = s;
    } else if (auto o = dynamic_cast<OverlayContainer*>(screen_or_overlay)) {
        overlaysContainer->addChild(o);
        o->onOpen();
        current = o;
    }

    updateTitle();
}

Screen* OsuGame::popManyScreens(int amount) {
    if (screenStack.size()==0) {
        return nullptr;
    }
    bool schedulePause = screenStack.size()==1;
    Screen* s = nullptr;
    auto oc = overlaysContainer->getChildren();
    int count = oc ? oc->count() : 0;
    for (;amount>0;amount--) {
        if (count!=0) {
            auto c = static_cast<OverlayContainer*>(oc->objectAtIndex(count-1));
            c->onClose();
            overlayPopQueue.push_back(c);
            count--;
        }
        else if (screenStack.size()!=0) s = screenStack.pop_back();
        else break;
    }
    Screen* ps = nullptr;
    if (screenStack.size()!=0) ps = screenStack[screenStack.size()-1];
    
    ScreenTransitionEvent event = {s,ps};
    s->onExiting(event);
    screenPopQueue.push_back(s);
    if (ps!=nullptr) ps->onEntering(event);
    current = ps;
    if (schedulePause) m_pActionManager->pauseTarget(this);

    updateTitle();
    
    return s;
}

Screen* OsuGame::popScreen() {return popManyScreens(1);}

template<typename T>
T* OsuGame::popUntilScreenType() {
    int idx = 0;
    for (Screen* s : screenStack) {
        if (dynamic_cast<T*>(s)) return popManyScreens(screenStack.size()-idx+overlaysContainer->getChildrenCount());
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
    std::vector<Screen*> e;
    for (auto* s : screenPopQueue) {
        if (m_pActionManager->numberOfRunningActionsInTarget(s) == 0) {
            screensContainer->removeChild(s);
            e.push_back(s);
        }
    };
    if (e.size()!=0) {
        auto i = screenPopQueue.inner();
        for (auto*s : screenPopQueue) {
            i->removeObject(s);
        }
    }

}

bool OsuGame::dispatchEvent(NodeEvent* event) {
    if (!isRunning()) return true;
    if (event->target() != nullptr) return false;
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
