#pragma once

#include "../../../frameworks/graphics/containers/VisibilityContainer.hpp"
#include "../../../frameworks/graphics/sprites/CCResizableSprite.hpp"

class LoadingSpinner final: public VisibilityContainer {
  CCResizableSprite* spinner;
  bool boxed = false;
public:
  const float TRANSITION_DURATION = 0.5;

  const float spin_duration = 0.9;
  
  bool init(bool boxed = true, bool invert = false);
  static LoadingSpinner* create(bool boxed = true, bool invert = false);
  void onOpen() override;
  void onClose() override;
};

