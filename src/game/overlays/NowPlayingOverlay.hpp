#pragma once

#include "../graphics/containers/OsuOverlayContainer.hpp"
#include "../../framework/audio/AudioManager.hpp"

class NowPlayingOverlay : public OsuOverlayContainer {
private:
  const float player_width = 400;
  const float player_height = 130;
  const float transition_length = 800;
  const float progress_height = 10;
  const float bottom_black_area_height = 55;
  const float margin = 10;

  AudioManager* ctrl;
public:
  bool init();
  $default_create(NowPlayingOverlay);

  void onOpen() override;
  void onClose() override;
};