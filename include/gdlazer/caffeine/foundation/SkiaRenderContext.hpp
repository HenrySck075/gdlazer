#pragma once

#include <memory>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkImage.h>

namespace caffeine {

/// Manages Skia rendering context for off-screen drawing.
/// Wraps SkSurface and provides a canvas for RenderObjects to paint on.
class SkiaRenderContext {
private:
  sk_sp<SkSurface> m_surface;
  sk_sp<SkImage> m_lastSnapshot;
  int m_width = 0;
  int m_height = 0;

public:
  SkiaRenderContext() = default;
  ~SkiaRenderContext() = default;

  /// Initialize/reset the rendering surface for a new frame.
  /// Creates an off-screen Skia surface with the given dimensions.
  void beginFrame(int width, int height);

  /// Finalize the current frame and snapshot the rendered image.
  /// Must be called after all painting is done.
  void endFrame();

  /// Get the canvas for painting operations.
  /// Only valid between beginFrame() and endFrame().
  SkCanvas* getCanvas();

  /// Get the snapshot of the last rendered frame.
  /// Valid after endFrame() has been called.
  sk_sp<SkImage> getSnapshot() const { return m_lastSnapshot; }

  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }
};

}  // namespace caffeine
