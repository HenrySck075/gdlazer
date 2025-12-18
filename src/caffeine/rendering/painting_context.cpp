#include "gdlazer/caffeine/rendering/painting_context.hpp"

namespace caffeine {

void PaintingContext::beginLayer(const Rect& bounds) {
  m_layer = std::make_unique<Layer>(bounds);
  m_recorder = std::make_unique<SkPictureRecorder>();
  m_recorder->beginRecording(bounds.width, bounds.height);
}

std::unique_ptr<Layer> PaintingContext::endLayer() {
  if (m_recorder) {
    sk_sp<SkPicture> picture = m_recorder->finishRecordingAsPicture();
    if (m_layer) {
      m_layer->setPicture(picture);
      m_layer->markClean();
    }
    m_recorder.reset();
  }
  return std::move(m_layer);
}

}  // namespace caffeine
