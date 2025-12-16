#include "gdlazer/caffeine/widgets/Align.hpp"
#include "gdlazer/caffeine/rendering/proxy_box/aligned_box.hpp"

namespace caffeine {

std::shared_ptr<RenderObject> Align::createRenderObject() {
  auto renderObj = std::make_shared<RenderPositionedBox>(
    std::dynamic_pointer_cast<Alignment>(m_alignment),
    nullptr,
    m_widthFactor,
    m_heightFactor
  );
  return renderObj;
}

void Align::updateRenderObject(std::shared_ptr<BuildContext> context,
                               std::shared_ptr<RenderObject> renderObject) {
  auto posBox = std::dynamic_pointer_cast<RenderPositionedBox>(renderObject);
  if (posBox) {
    posBox->setAlignment(m_alignment);
    posBox->setWidthFactor(m_widthFactor);
    posBox->setHeightFactor(m_heightFactor);
  }
}

}  // namespace caffeine
