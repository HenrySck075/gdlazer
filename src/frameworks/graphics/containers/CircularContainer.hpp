#pragma once

#include "Container.hpp"
#include <cmath>

GDF_NS_START

/// A Container which is rounded on the shortest edge
class CircularContainer : public Container {
public:
  $default_create(CircularContainer);

  void update(float dt) override {
    float radius = std::fminf(getContentWidth(), getContentHeight()) / 2.f;
    float oldRadius = getBorderRadius();
    if (radius != oldRadius) setBorderRadius(radius);
  }
};

GDF_NS_END
