#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../utils.hpp"
#include "../framework/graphics/containers/Container.hpp"
#include "../framework/graphics/containers/EventTarget.hpp"
using namespace cocos2d;

// widgets
#include "screens/select/SongSelect.hpp"
#include "overlays/toolbar/Toolbar.hpp"

// the
class OsuScreen;
#include "../game/screens/OsuScreen.hpp"
#include "../framework/screens/ScreenTransitionEvent.hpp"

// the grand dad
//
// funny
class OsuGame : public CCScene, public EventTarget {
private:
    static OsuGame* instance;

    Toolbar* toolbar;
    Container* main;

    CCArrayExt<OsuScreen*> screenStack;
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

    bool init();

    void showToolbar();
    void hideToolbar();

    void pushScreen(OsuScreen* s);
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