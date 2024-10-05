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
    CCArrayExt<OverlayContainer*> overlayStack;

    float bgVol = 1;
    float sfxVol = 1;
    float volMult = 1;
    bool volMultChange = false;

    /// can be overlay or screen
    Container* current = nullptr;
  
    /// preloaded overlays
    CCDictionaryExt<std::string, OverlayContainer*> overlays;

    /// list of level songs to play
    CCArrayExt<GJGameLevel*> mainPlaylist;
    
public:
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


    bool dispatchEvent(NodeEvent* event) override;

    bool init();

    void showToolbar();
    void hideToolbar();

    void showSettings();
    void hideSettings();
    // push screen and return the pushed screen if one needs to do more stuff with it (does not include removing the screen)
    Screen* pushScreen(Screen* s);
    Screen* replaceScreen(Screen* s);
    Screen* popManyScreens(int amount = 1, bool popOverlays = true);
    Screen* popScreen(bool popOverlays = true);
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
