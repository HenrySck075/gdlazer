#pragma once

#include "../graphics/containers/OsuOverlayContainer.hpp"
#include "MusicController.hpp"

class NowPlayingOverlay : public OsuOverlayContainer {
private:
    const float player_width = 400;
    const float player_height = 130;
    const float transition_length = 800;
    const float progress_height = 10;
    const float bottom_black_area_height = 55;
    const float margin = 10;

    MusicController* ctrl;
public:
    bool init();
};