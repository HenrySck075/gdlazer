#include "gdlazer/caffeine/widgets/Align.hpp"
#include "gdlazer/caffeine/rendering/proxy_box/aligned_box.hpp"

namespace caffeine {

RefNauseam<RenderObject> Align::createRenderObject() {
  auto renderObj = new RenderPositionedBox(
    std::dynamic_pointer_cast<Alignment>(m_alignment),
    nullptr,
    m_widthFactor,
    m_heightFactor
  );
  return renderObj;
}

void Align::updateRenderObject(RefNauseam<BuildContext> context,
                               RefNauseam<RenderObject> renderObject) {
  auto posBox = dynamic_cast<RenderPositionedBox*>(renderObject.get());
  if (posBox) {
    posBox->setAlignment(m_alignment);
    posBox->setWidthFactor(m_widthFactor);
    posBox->setHeightFactor(m_heightFactor);
  }
}

}  // namespace caffeine
