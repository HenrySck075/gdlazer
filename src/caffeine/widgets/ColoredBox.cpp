#include "gdlazer/caffeine/widgets/ColoredBox.hpp"
#include <skia/include/core/SkPaint.h>

namespace caffeine {

void RenderColoredBox::paint(PaintingContext* context, const Offset& offset) {
  if (context) {
    SkPaint paint;
    uint8_t a = m_color.getAlpha();
    uint8_t r = m_color.getRed();
    uint8_t g = m_color.getGreen();
    uint8_t b = m_color.getBlue();
    
    paint.setColor(SkColorSetARGB(a, r, g, b));
    paint.setAntiAlias(m_isAntiAlias);

    SkRect rect = SkRect::MakeWH(m_size.width, m_size.height);
    auto canvas = context->getCanvas(); canvas->drawRect(rect, paint);
  }

  RenderProxyBox::paint(context, offset);
}

RefNauseam<RenderObject> ColoredBox::createRenderObject() {
  return new RenderColoredBox(m_color, m_isAntiAlias);
}

void ColoredBox::updateRenderObject(RefNauseam<BuildContext> context,
                                    RefNauseam<RenderObject> renderObject) {
  auto coloredBox = dynamic_cast<RenderColoredBox*>(renderObject.get());
  if (coloredBox) {
    geode::log::debug("hello");
    coloredBox->setColor(m_color);
    coloredBox->setIsAntiAlias(m_isAntiAlias);
  }
}

}  // namespace caffeine
