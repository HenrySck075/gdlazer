#pragma once

#include "../graphics/containers/OsuOverlayContainer.hpp"


class SettingsPanel : public OsuOverlayContainer {
    float const TRANSITION_LENGTH = 0.6;
    /// what this doin
    float ExpandedPosition = 0;
    float const PANEL_WIDTH = 7;
    float const SIDEBAR_WIDTH = 400;
    float const WIDTH = PANEL_WIDTH + SIDEBAR_WIDTH;
public:
    static SettingsPanel* create() {
        create_class(SettingsPanel, init);
    }
    bool init();

    void onOpen() override;
    void onClose() override;
};
