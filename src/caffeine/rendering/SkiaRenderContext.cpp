#include "gdlazer/caffeine/rendering/SkiaRenderContext.hpp"

namespace caffeine {

void SkiaRenderContext::beginFrame(int width, int height) {
  if (m_width != width || m_height != height || !m_surface) {
    m_width = width;
    m_height = height;

    // Create a raster surface (in-memory, RGBA8888)
    SkImageInfo info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kOpaque_SkAlphaType);
    m_surface = SkSurfaces::Raster(info);

    if (!m_surface) {
      // Fallback or error handling
      return;
    }
  }

  // Clear the canvas to white
  SkCanvas* canvas = m_surface->getCanvas();
  if (canvas) {
    canvas->clear(SK_ColorBLACK);
  }
}

void SkiaRenderContext::endFrame() {
  if (m_surface) {
    // Take a snapshot of the rendered surface
    m_lastSnapshot = m_surface->makeImageSnapshot();
  }
}

SkCanvas* SkiaRenderContext::getCanvas() {
  if (m_surface) {
    return m_surface->getCanvas();
  }
  return nullptr;
}

}  // namespace caffeine
