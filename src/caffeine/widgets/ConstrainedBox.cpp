#include "gdlazer/caffeine/widgets/ConstrainedBox.hpp"
#include "gdlazer/caffeine/rendering/proxy_box/sizer.hpp"

namespace caffeine {

RefNauseam<RenderObject> ConstrainedBox::createRenderObject() {
  return new RenderConstrainedBox(m_constraints);
}

void ConstrainedBox::updateRenderObject(RefNauseam<BuildContext> context,
                                        RefNauseam<RenderObject> renderObject) {
  auto constrainedBox = dynamic_cast<RenderConstrainedBox*>(renderObject.get());
  if (constrainedBox) {
    constrainedBox->setAdditionalConstraints(m_constraints);
  }
}

}  // namespace caffeine
