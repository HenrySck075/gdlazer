#pragma once

#include <memory>
#include <functional>
#include <skia/include/core/SkPicture.h>
#include <skia/include/core/SkCanvas.h>
#include "../dartui/basic_types.hpp"

namespace caffeine {

/// Represents a cached layer for a repaint boundary.
/// Stores a picture (recorded drawing commands) that can be replayed
/// without needing to re-execute the entire paint call.
class Layer {
private:
  sk_sp<SkPicture> m_picture;
  std::reference_wrapper<const Rect> m_bounds;
  bool m_isDirty = true;

public:
  explicit Layer(const Rect& bounds) : m_bounds(std::cref(bounds)) {}

  const Rect& getBounds() const { return m_bounds.get(); }

  sk_sp<SkPicture> getPicture() const { return m_picture; }
  void setPicture(sk_sp<SkPicture> picture) { m_picture = picture; }

  bool isDirty() const { return m_isDirty; }
  void markDirty() { m_isDirty = true; }
  void markClean() { m_isDirty = false; }

  /// Draw this layer's cached picture to the given canvas
  void draw(SkCanvas* canvas) {
    if (canvas && m_picture) {
      m_picture->playback(canvas);
    }
  }
};

}  // namespace caffeine
