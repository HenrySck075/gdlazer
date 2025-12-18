#include "gdlazer/caffeine/widgets/Opacity.hpp"
#include <skia/include/core/SkPaint.h>

namespace caffeine {

void RenderOpacity::paint(PaintingContext* context, const Offset& offset) {
  if (!context || m_opacity == 0.0) {
    // Fully transparent - don't paint anything
    return;
  }

  if (m_opacity == 1.0) {
    // Fully opaque - paint normally
    RenderProxyBox::paint(context, offset);
    return;
  }

  // Partially transparent - paint with opacity
  if (m_child) {
    auto canvas = context->getCanvas();
    canvas->saveLayerAlpha(nullptr, static_cast<int>(m_opacity * 255));
    m_child->paint(context, offset);
    canvas->restore();
  }
}

RefNauseam<RenderObject> Opacity::createRenderObject() {
  return new RenderOpacity(m_opacity, m_alwaysIncludeSemantics);
}

void Opacity::updateRenderObject(RefNauseam<BuildContext> context,
                                 RefNauseam<RenderObject> renderObject) {
  auto opacity = dynamic_cast<RenderOpacity*>(renderObject.get());
  if (opacity) {
    opacity->setOpacity(m_opacity);
    opacity->setAlwaysIncludeSemantics(m_alwaysIncludeSemantics);
  }
}

}  // namespace caffeine
