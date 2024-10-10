#pragma once

#include "../../OsuGame.hpp"
#include "../../../framework/graphics/containers/OverlayContainer.hpp"

class OsuOverlayContainer : public OverlayContainer {
    bool shown = false;
public:
    void show() override {
        if (shown) return;
        shown = true;
        OsuGame::get()->pushOverlay(this);
    }
    void hide() override {
        if (!shown) return;
        shown = false;
        OverlayContainer::hide();
    }
};
