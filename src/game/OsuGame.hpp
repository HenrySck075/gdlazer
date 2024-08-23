#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../utils.hpp"
#include "../framework/graphics/containers/Container.hpp"
#include "../framework/graphics/containers/EventTarget.hpp"
using namespace cocos2d;

// widgets
#include "overlays/toolbar/Toolbar.hpp"

// the
class OsuScreen;
#include "../framework/screens/Screen.hpp"
#include "../framework/screens/ScreenTransitionEvent.hpp"

// the grand dad
//
// funny
class OsuGame : public CCScene, public EventTarget {
private:
    static OsuGame* instance;

    Toolbar* toolbar;

    CCArrayExt<Screen*> screenStack;
    CCArrayExt<Screen*> screenPopQueue;
    Container* main;

    CCRepeatForever* checkAction;
public:
    Screen* currentScreen = nullptr;
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

    void e() {
        auto m = CCDirector::sharedDirector()->getOpenGLView()->getDesignResolutionSize();
        if (m!=getContentSize()) {
            setContentSize(m);
            dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
        }
    }

public:
    bool dispatchEvent(NodeEvent* event) override {
        if (event->getCaller() != nullptr) return false;
        if (event->eventName().starts_with("og")) {
            EventTarget::dispatchEvent(event);
            return true;
        }
        if (CCDirector::sharedDirector()->getRunningScene()!=this) return true;
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
        if (currentScreen) currentScreen->dispatchEvent(event);
        return true;
    }

    bool init();

    void showToolbar();
    void hideToolbar();

    void pushScreen(Screen* s);
    Screen* popScreen();

    void onLoseFocus();
    void onFocus();

    void checkForQueue();
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