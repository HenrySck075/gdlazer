#pragma once

#include "../graphics/containers/WaveContainer.hpp"

GDL_NS_START

class ModsOverlay : public WaveContainer {
public:
  $defaultCreate(ModsOverlay);

  bool init() override;
};

GDL_NS_END