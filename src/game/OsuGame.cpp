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
    main = Container::create();
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
    Screen* ls;
    if (screenStack.size()!=0) {
        ls = screenStack[screenStack.size()-1];
    }
    s->onEntering(ScreenTransitionEvent(ls,s));
    screenStack.push_back(s);
    main->addChild(s);
    
}

void OsuGame::popScreen() {
    if (screenStack.size()==0) {
        log::error("[OsuGame]: nice >:]");
        return;
    }
    auto s = screenStack[screenStack.size()-1];
    auto ps = screenStack.pop_back();
    
    ScreenTransitionEvent event = {s,ps};
    s->onExiting(event);
    if (ps!=nullptr) ps->onEntering(event);
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

#include <Geode/modify/AppDelegate.hpp>

class $modify(AppDelegate) {
    void platformShutdown() {
        log::info("[hook: AppDelegate]: shutdown app");
        OsuGame::get()->release();
        AppDelegate::platformShutdown();
    }
};