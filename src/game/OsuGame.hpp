#pragma once

#include <Geode/cocos/include/cocos2d.h>
#include "../utils.hpp"
#include "../framework/graphics/containers/Container.hpp"
using namespace cocos2d;

// widgets
#include "screens/menu/MainMenu.hpp"
#include "overlays/toolbar/Toolbar.hpp"

// the grand dad
//
// funny
class OsuGame : public CCScene {
private:
    Toolbar* toolbar;
    Container* main;
public:
    static OsuGame* create() {
        OsuGame* ret = new OsuGame(); 
        if (ret && ret->init()) { ret->autorelease(); } 
        else { 
            do { if (ret) { (ret)->release(); (ret) = 0; } } while (0); 
        }; 
        return ret;
    }

    bool init();

    void showToolbar();
    void hideToolbar();
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