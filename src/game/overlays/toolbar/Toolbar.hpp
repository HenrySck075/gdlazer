#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "../../../framework/graphics/containers/Container.hpp"
#include "../../graphics/OsuColor.hpp"
#include "ToolbarConstants.hpp"
using namespace geode::prelude;

class Toolbar : public Container {
private:
    double const transition_time = 0.5;
public:
    
    static Toolbar* create() {
        create_class(Toolbar, init);
    }
    bool init() override;

    void show();
    void hide();
};