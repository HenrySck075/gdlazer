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

  CCPoint m_startTouchLocation = {0,0};
  CCRect m_touchBoundary;

  CCDrawNode* m_wave1;
  CCDrawNode* m_wave2;
  CCDrawNode* m_wave3;
  CCDrawNode* m_wave4;

  geode::Ref<Container> m_body; /// TODO: delete this?

  float m_pos1;
  float m_pos2;
  float m_pos3;
  float m_pos4;

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
protected:
  geode::Ref<OverlayColorProvider> m_provider;

};
GDL_NS_END