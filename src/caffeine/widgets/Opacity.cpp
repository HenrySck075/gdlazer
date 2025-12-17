#include "gdlazer/caffeine/widgets/Opacity.hpp"
#include <skia/include/core/SkPaint.h>

namespace caffeine {

void RenderOpacity::paint(SkCanvas* canvas) {
  if (!canvas || m_opacity == 0.0) {
    // Fully transparent - don't paint anything
    return;
  }

  if (m_opacity == 1.0) {
    // Fully opaque - paint normally
    RenderProxyBox::paint(canvas);
    return;
  }

  // Partially transparent - paint with opacity
  if (m_child) {
    canvas->saveLayerAlpha(nullptr, static_cast<int>(m_opacity * 255));
    m_child->paint(canvas);
    canvas->restore();
  }
}

std::shared_ptr<RenderObject> Opacity::createRenderObject() {
  return std::make_shared<RenderOpacity>(m_opacity, m_alwaysIncludeSemantics);
}

void Opacity::updateRenderObject(std::shared_ptr<BuildContext> context,
                                 std::shared_ptr<RenderObject> renderObject) {
  auto opacity = std::dynamic_pointer_cast<RenderOpacity>(renderObject);
  if (opacity) {
    opacity->setOpacity(m_opacity);
    opacity->setAlwaysIncludeSemantics(m_alwaysIncludeSemantics);
  }
}

}  // namespace caffeine
