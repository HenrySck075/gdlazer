#pragma once

#include <Geode/Geode.hpp>
#include "../../../utils.hpp"
#include "../../../framework/graphics/containers/Container.hpp"
#include "../../graphics/OsuColor.hpp"
#include "../../graphics/CCLayerGradient2.hpp"
#include "ToolbarConstants.hpp"

using namespace geode::prelude;


class Toolbar : public Container {
private:
    double const transition_time = 0.5;
    CCLayerGradient2* gradient;
public:
    
    static Toolbar* create() {
        create_class(Toolbar, init);
    }
    bool init() override;

    void show();
    void hide();

    void onMouseEnter() override {
        gradient->runAction(CCFadeTo::create(0.5,255));
    }
    void onMouseExit() override {
        gradient->runAction(CCFadeTo::create(0.5,0));
    }
};