#pragma once

#include "Container.hpp"
#include <cmath>

GDF_NS_START

struct CircularContainerImpl {
  static void updateBorder(Container* container) {
    float radius = std::fminf(container->getContentWidth(), container->getContentHeight()) / 2.f;
    float oldRadius = container->getBorderRadius();
    if (radius != oldRadius) container->setBorderRadius(radius);
  }
};

/// A Container which is rounded on the shortest edge
class CircularContainer : public Container {
public:
  $default_create(CircularContainer);

  void update(float dt) override {
    CircularContainerImpl::updateBorder(this);
  }
};

GDF_NS_END
