#pragma once

#include "../../OsuGame.hpp"
#include "../../../framework/graphics/containers/OverlayContainer.hpp"

class OsuOverlayContainer : public OverlayContainer {
public:
    void show() override {
        OverlayContainer::show();
        OsuGame::get()->pushOverlay(this);
    }
    void hide() override {
        OverlayContainer::hide();
        OsuGame::get()->popOverlay(this);
    }
};
