#pragma once

#include "../../OsuGame.hpp"
#include "../../../framework/graphics/containers/OverlayContainer.hpp"

class OsuOverlayContainer : public OverlayContainer {
public:
    void show() override {
        OsuGame::get()->pushOverlay(this);
    }
    void hide() override {
        // TODO: f
        OsuGame::get()->popOverlay();
    }
    void onClick(MouseEvent* e) override {
        if (!boundingBoxFromContentSize(main).containsPoint(e->position)) OsuGame::get()->popOverlay();
    }
};
