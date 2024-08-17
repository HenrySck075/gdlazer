#include "OsuGame.hpp"
#include "../helpers/CustomActions.hpp"

#ifdef GEODE_IS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

LONG_PTR oWindowProc;
bool newWindowProcSet = false;

LRESULT CALLBACK nWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_SIZE:
    case WM_MOVE:
    case WM_KILLFOCUS:
        if (OsuGame* mainGame = typeinfo_cast<OsuGame*>(OsuGame::get())) {
            mainGame->onLoseFocus();
        }
        break;
    case WM_SETFOCUS:
        if (OsuGame* mainGame = typeinfo_cast<OsuGame*>(OsuGame::get())) {
            mainGame->onFocus();
        }
        break;
    }

    return CallWindowProc((WNDPROC)oWindowProc, hwnd, msg, wparam, lparam);
}

HWND getWindowHandle() {
    return WindowFromDC(wglGetCurrentDC());
}
#endif

OsuGame* OsuGame::instance = nullptr;

bool OsuGame::init() {
    CCScene::init();
    checkAction = CCRepeatForever::create(
        CCSequence::createWithTwoActions(
            CCDelayTime::create(0.1),
            CCCallFunc::create(this,callfunc_selector(OsuGame::checkForQueue))
        )
    );
    m_pActionManager->pauseTarget(this);
    main = Container::create();
    main->setID("mainUI");
    main->setContentSize(getContentSize());
    this->addChild(main);
    toolbar = Toolbar::create();
    this->addChild(toolbar);
#ifdef GEODE_IS_WINDOWS
    if (!newWindowProcSet) {
        oWindowProc = SetWindowLongPtrA(getWindowHandle(), -4, (LONG_PTR)nWindowProc);
        newWindowProcSet = true;
    }
#endif
    addListener("ogExitDidFinish", [this](NodeEvent* e){
        main->removeChild(static_cast<ScreenTransitionNotifier*>(e)->caller);
    });
    runAction(checkAction);
    return true;
}

void OsuGame::showToolbar() {
    toolbar->show();
    main->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,main->getContentWidth(),main->getContentHeight()-toolbar->HEIGHT)
    ));
}

void OsuGame::hideToolbar() {
    toolbar->hide();
    main->runAction(CCEaseOutQuint::create(
        CCResizeTo::create(0.5,main->getContentWidth(),getContentHeight())
    ));
}

void OsuGame::pushScreen(Screen* s) {
    Screen* ls = nullptr;
    bool scheduleResume = screenStack.size()==0;
    if (screenStack.size()!=0) {
        ls = screenStack[screenStack.size()-1];
    }
    auto e = ScreenTransitionEvent(ls,s);
    if (ls) ls->onExiting(e);
    s->onEntering(e);
    screenStack.push_back(s);
    main->addChild(s);
    currentScreen = s;
    if (scheduleResume) m_pActionManager->resumeTarget(this);
}

Screen* OsuGame::popScreen() {
    if (screenStack.size()==0) {
        log::error("[OsuGame]: nice >:]");
        return nullptr;
    }
    bool schedulePause = screenStack.size()==1;
    auto s = screenStack.pop_back();
    Screen* ps = nullptr;
    if (screenStack.size()!=0) ps = screenStack[screenStack.size()-1];
    
    ScreenTransitionEvent event = {s,ps};
    s->onExiting(event);
    screenPopQueue.push_back(s);
    if (ps!=nullptr) ps->onEntering(event);
    currentScreen = ps;
    if (schedulePause) m_pActionManager->pauseTarget(this);
    
    return s;
}

void OsuGame::onLoseFocus() {
    auto engine = FMODAudioEngine::sharedEngine();
    engine->setBackgroundMusicVolume(engine->getBackgroundMusicVolume()*0.6);
    engine->setEffectsVolume(engine->getEffectsVolume()*0.6);
}
void OsuGame::onFocus() {
    auto engine = FMODAudioEngine::sharedEngine();
    engine->setBackgroundMusicVolume(engine->getBackgroundMusicVolume()/0.6);
    engine->setEffectsVolume(engine->getEffectsVolume()/0.6);
}

void OsuGame::checkForQueue() {
    std::vector<Screen*> e;
    for (auto* s : screenPopQueue) {
        if (m_pActionManager->numberOfRunningActionsInTarget(s) == 0) {
            main->removeChild(s);
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


#include <Geode/modify/AppDelegate.hpp>

class $modify(AppDelegate) {
    void platformShutdown() {
        log::info("[hook: AppDelegate]: shutdown app");
        OsuGame::get()->release();
        AppDelegate::platformShutdown();
    }
};