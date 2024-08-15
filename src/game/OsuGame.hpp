#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../utils.hpp"
#include "../framework/graphics/containers/Container.hpp"
#include "../framework/graphics/containers/EventTarget.hpp"
using namespace cocos2d;

// widgets
#include "overlays/toolbar/Toolbar.hpp"

// the
class Screen;
#include "../framework/screens/Screen.hpp"
#include "../framework/screens/ScreenTransitionEvent.hpp"

// the grand dad
//
// funny
class OsuGame : public CCScene, public EventTarget {
private:
    static OsuGame* instance;

    Toolbar* toolbar;
    Container* main;

    CCArrayExt<Screen*> screenStack;
    Screen* currentScreen;
public:
    // TODO: RELEASE ON GAME EXIT (if anyone caused a game crash or close via the console then FEAR THE LEAK)
    // (its not scary most users dont leave their pc overnight anyways)
    static OsuGame* get() {
        if (instance == nullptr) {
            OsuGame* ret = new OsuGame(); 
            if (ret && ret->init()) { ret->autorelease(); } 
            else { 
                do { if (ret) { (ret)->release(); (ret) = 0; } } while (0); 
            }; 
            instance = ret;
            instance->retain();
        }
        return instance;
    }

    void dispatchEvent(NodeEvent* event) override {
        if (event->eventName() == "mouseEvent") {
            updateDispatchFlow(event, DispatchingFlow::Down);
            toolbar->dispatchEvent(event);
            if (currentScreen) currentScreen->dispatchEvent(event);
            return;
        }
        EventTarget::dispatchEvent(event);
    }

    bool init();

    void showToolbar();
    void hideToolbar();

    void pushScreen(Screen* s);
    void popScreen();

    void onLoseFocus();
    void onFocus();
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