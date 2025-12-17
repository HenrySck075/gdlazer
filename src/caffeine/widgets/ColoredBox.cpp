#include "gdlazer/caffeine/widgets/ColoredBox.hpp"
#include <skia/include/core/SkPaint.h>

namespace caffeine {

void RenderColoredBox::paint(SkCanvas* canvas) {
  if (canvas) {
    SkPaint paint;
    uint8_t a = m_color.getAlpha();
    uint8_t r = m_color.getRed();
    uint8_t g = m_color.getGreen();
    uint8_t b = m_color.getBlue();
    
    paint.setColor(SkColorSetARGB(a, r, g, b));
    paint.setAntiAlias(m_isAntiAlias);

    SkRect rect = SkRect::MakeWH(m_size.width, m_size.height);
    canvas->drawRect(rect, paint);
  }

  RenderProxyBox::paint(canvas);
}

std::shared_ptr<RenderObject> ColoredBox::createRenderObject() {
  return std::make_shared<RenderColoredBox>(m_color, m_isAntiAlias);
}

void ColoredBox::updateRenderObject(std::shared_ptr<BuildContext> context,
                                    std::shared_ptr<RenderObject> renderObject) {
  auto coloredBox = std::dynamic_pointer_cast<RenderColoredBox>(renderObject);
  if (coloredBox) {
    coloredBox->setColor(m_color);
    coloredBox->setIsAntiAlias(m_isAntiAlias);
  }
}

}  // namespace caffeine
