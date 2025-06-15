#pragma once

#include <Geode/Geode.hpp>
#include "../../overlays/OverlayColorProvider.hpp"
#include "OsuOverlayContainer.hpp"
using namespace geode::prelude;

GDL_NS_START
using ColorScheme = OverlayColorScheme;

class WaveContainer : public OsuOverlayContainer {
private:
  CCPoint m_startTouchLocation = {0,0};
  CCRect m_touchBoundary;

  CCDrawNode* m_wave1 = nullptr;
  CCDrawNode* m_wave2 = nullptr;
  CCDrawNode* m_wave3 = nullptr;
  CCDrawNode* m_wave4 = nullptr;

  float m_pos1;
  float m_pos2;
  float m_pos3;
  float m_pos4;

  float appearDuration = 0.8f;
  float disappearDuration = 0.5f;

  void onClose() override;
  void onOpen() override;
public:
  bool init(ColorScheme color);
  static WaveContainer* create(ColorScheme color);
protected:
  geode::Ref<OverlayColorProvider> m_provider;

};
GDL_NS_END