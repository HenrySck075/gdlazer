#pragma once

#include "../../OsuGame.hpp"
#include "../../../framework/graphics/containers/OverlayContainer.hpp"

class OsuOverlayContainer : public OverlayContainer {
    bool shown = false;
public:
    void show() override {
        shown = true;
        OsuGame::get()->pushOverlay(this);
    }
    void hide() override {
        shown = false;
        // TODO: f
        OsuGame::get()->popOverlay();
    }
};
