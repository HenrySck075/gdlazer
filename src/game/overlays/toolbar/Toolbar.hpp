#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../graphics/CCResizableSprite.hpp"
#include <henrysck075.reactiveui/include/style.hpp>
#include <henrysck075.reactiveui/include/enum.hpp>
using namespace geode::prelude;

class Toolbar : public CCLayer {
public:
    Style* toolbarStyle;
    float const HEIGHT = 40;
    float const TOOLTIP_HEIGHT = 30;

    static Toolbar* create() {
        create_class(Toolbar, init);
    }
    bool init();
};