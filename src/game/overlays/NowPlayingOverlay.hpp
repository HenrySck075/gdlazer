#pragma once

#include "../graphics/containers/OsuOverlayContainer.hpp"
#include "../../frameworks/audio/AudioManager.hpp"

GDL_NS_START
class NowPlayingOverlay : public OsuOverlayContainer {
private:
  const float c_playerWidth = 400;
  const float c_playerHeight = 130;
  const float c_transitionLength = 0.8;
  const float c_progressHeight = 10;
  const float c_bottomBlackAreaHeight = 55;
  const float margin = 10;

  frameworks::AudioManager* ctrl;
public:
  bool init();
  $default_create(NowPlayingOverlay);

  void onOpen() override;
  void onClose() override;
};
GDL_NS_END
