#pragma once

#include "../../OsuGame.hpp"
#include "../../../framework/graphics/containers/OverlayContainer.hpp"

class OsuOverlayContainer : public OverlayContainer {
public:
    void show() override {
        OsuGame::get()->pushScreen(this);
    }
    void hide() override {
        // not really reliable
        OsuGame::get()->popScreen();
    }
};