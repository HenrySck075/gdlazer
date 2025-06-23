#pragma once

#include "../graphics/containers/WaveContainer.hpp"
#include "geodemods/SwelvyBG.hpp"

GDL_NS_START

class ModsOverlay : public WaveContainer {
public:
  $defaultCreate(ModsOverlay);

  bool init() override;

  void onOpen() override;
  void onClose() override;

private:
  Container* createInstalledTab();
  Container* createPopularTab();

  geode::Ref<SwelvyBG> m_bg;
};

GDL_NS_END