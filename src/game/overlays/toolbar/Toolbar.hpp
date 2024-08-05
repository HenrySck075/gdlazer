#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "../../../framework/graphics/containers/Container.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../graphics/CCResizableSprite.hpp"
using namespace geode::prelude;

class Toolbar : public Container {
public:
    float const HEIGHT = 40;
    float const TOOLTIP_HEIGHT = 30;

    static Toolbar* create() {
        create_class(Toolbar, init);
    }
    bool init();
};