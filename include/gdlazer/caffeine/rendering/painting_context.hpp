#pragma once

#include <skia/include/core/SkCanvas.h>
#include "../dartui/basic_types.hpp"

namespace caffeine {

/// Wraps painting state for a render object.
/// Mirrors Flutter's PaintingContext which provides abstraction
/// over the canvas and allows for layer/compositing logic.
class PaintingContext {
private:
  SkCanvas* m_canvas;
  Rect m_paintBounds;

public:
  PaintingContext(SkCanvas* canvas, const Rect& paintBounds)
    : m_canvas(canvas), m_paintBounds(paintBounds) {}

  SkCanvas* getCanvas() const { return m_canvas; }
  const Rect& getPaintBounds() const { return m_paintBounds; }

  /// Apply an offset transformation to the canvas
  /// Caller must restore() after painting
  void pushOffset(const Offset& offset) {
    if (m_canvas) {
      m_canvas->save();
      m_canvas->translate(offset.dx, offset.dy);
    }
  }

  /// Restore the canvas state after pushOffset
  void popOffset() {
    if (m_canvas) {
      m_canvas->restore();
    }
  }
};

}  // namespace caffeine

