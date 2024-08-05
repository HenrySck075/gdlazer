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
public:
    static OsuGame* create() {
        create_class(OsuGame, init);
    }
    bool init();
};