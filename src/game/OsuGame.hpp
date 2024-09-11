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
#include "../framework/graphics/containers/OverlayContainer.hpp"

// the grand dad
//
// funny
class OsuGame : public CCScene, public EventTarget {
private:
    float offset = 0;
    static OsuGame* instance;

    Toolbar* toolbar;

    CCArrayExt<Screen*> screenStack;
    CCArrayExt<Screen*> screenPopQueue;
    Container* screensContainer;
    Container* overlaysContainer;
    CCArrayExt<OverlayContainer*> overlayPopQueue;


    float bgVol = 1;
    float sfxVol = 1;
    float volMult = 1;
    bool volMultChange = false;

    // be responsible for your action and do NOT change these value yourself
    // 
    // can be overlay or screen
    Container* current = nullptr;
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

    void e() {
        auto m = CCDirector::sharedDirector()->getOpenGLView()->getDesignResolutionSize();
        if (m!=getContentSize()) {
            setContentSize(m);
            dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
        }
    }


public:
    bool dispatchEvent(NodeEvent* event) override;

    bool init();

    void showToolbar();
    void hideToolbar();

    void pushScreen(Screen* s);
    Screen* popManyScreens(int amount = 1);
    Screen* popScreen();
    void pushOverlay(OverlayContainer* o);
    OverlayContainer* popManyOverlays(int amount = 1);
    OverlayContainer* popOverlay();
    template<typename T>
    T* popUntilScreenType();

    void onLoseFocus();
    void onFocus();

    void checkForQueue();

    void setContentSize(CCSize const& size) override {
        CCNode::setContentSize(size);
        dispatchEvent(new NodeLayoutUpdate(NodeLayoutUpdateType::Size));
    }
    // on android, this does nothing
    void updateTitle();

    void update(float dt);
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
