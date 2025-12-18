#pragma once

#include <memory>
#include <functional>
#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkPictureRecorder.h>
#include "../dartui/basic_types.hpp"
#include "layer.hpp"

namespace caffeine {

/// Wraps painting state for a render object.
/// Mirrors Flutter's PaintingContext which provides abstraction
/// over the canvas and allows for layer/compositing logic.
class PaintingContext {
private:
  SkCanvas* m_canvas;
  std::reference_wrapper<const Rect> m_paintBounds;
  std::unique_ptr<SkPictureRecorder> m_recorder;
  std::unique_ptr<Layer> m_layer;

public:
  PaintingContext(SkCanvas* canvas, const Rect& paintBounds)
    : m_canvas(canvas), m_paintBounds(std::cref(paintBounds)) {}

  SkCanvas* getCanvas() const { return m_canvas; }
  const Rect& getPaintBounds() const { return m_paintBounds.get(); }

  /// Apply an offset transformation to the canvas
  /// Caller must popOffset() after painting
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

  /// Start recording a layer (for repaint boundaries)
  void beginLayer(const Rect& bounds);

  /// Finish recording and return the cached layer
  std::unique_ptr<Layer> endLayer();

  /// Draw a cached layer to the canvas
  void drawLayer(Layer* layer) {
    if (layer && m_canvas) {
      layer->draw(m_canvas);
    }
  }
};

}  // namespace caffeine

