#pragma once

#include "../graphics/containers/OsuOverlayContainer.hpp"
#include "../../frameworks/audio/AudioManager.hpp"

GDL_NS_START
class NowPlayingOverlay : public OsuOverlayContainer {
private:
  static constexpr float c_playerWidth = 400;
  static constexpr float c_playerHeight = 130;
  static constexpr float c_transitionLength = 0.8;
  static constexpr float c_progressHeight = 10;
  static constexpr float c_bottomBlackAreaHeight = 55;
  static constexpr float c_margin = 10;

  frameworks::AudioManager* m_ctrl = nullptr;

  float m_currentSongDuration = 0;

  frameworks::Container* m_bar = nullptr;
public:
  bool init();
  $defaultCreate(NowPlayingOverlay);

  void onOpen() override;
  void onClose() override;

  void update(float) override;
};
GDL_NS_END
