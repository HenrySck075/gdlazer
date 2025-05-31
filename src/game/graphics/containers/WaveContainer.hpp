#pragma once

#include <Geode/Geode.hpp>
#include "../../overlays/OverlayColorProvider.hpp"
#include "OsuOverlayContainer.hpp"
using namespace geode::prelude;

GDL_NS_START
using ColorScheme = OverlayColorScheme;

class WaveContainer : public OsuOverlayContainer {
private:
  bool m_hiding = false;

  OverlayColorProvider* m_provider;

  CCPoint m_startTouchLocation = {0,0};
  CCRect m_touchBoundary;

  CCDrawNode* m_wave1;
  CCDrawNode* m_wave2;
  CCDrawNode* m_wave3;
  CCDrawNode* m_wave4;

  Container* m_body;

  float pos1;
  float pos2;
  float pos3;
  float pos4;

  float appearDuration = 0.8f;
  float disappearDuration = 0.5f;

  CCDrawNode* createWave(float w, CCSize size, float angle, ccColor4B col);
  void onClose() override;
  void onOpen() override;
public:
  bool init(ColorScheme color, Container* body);
  // @note RobTop addition
  bool customSetup(Container* body);
  static WaveContainer* create(ColorScheme color, Container* body);
};
GDL_NS_END