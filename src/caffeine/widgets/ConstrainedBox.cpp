#include "gdlazer/caffeine/widgets/ConstrainedBox.hpp"

namespace caffeine {

void RenderConstrainedBox::performLayout() {
  if (m_child) {
    // Enforce additional constraints on top of our constraints
    BoxConstraints childConstraints = m_constraints.enforce(m_additionalConstraints);
    m_child->layout(childConstraints, true);
    m_size = m_constraints.constrain(m_child->getSize());
  } else {
    m_size = m_constraints.constrain(m_additionalConstraints.constrain(Size(0, 0)));
  }
}

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
