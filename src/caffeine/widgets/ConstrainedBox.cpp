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

std::shared_ptr<RenderObject> ConstrainedBox::createRenderObject() {
  return std::make_shared<RenderConstrainedBox>(m_constraints);
}

void ConstrainedBox::updateRenderObject(std::shared_ptr<BuildContext> context,
                                        std::shared_ptr<RenderObject> renderObject) {
  auto constrainedBox = std::dynamic_pointer_cast<RenderConstrainedBox>(renderObject);
  if (constrainedBox) {
    constrainedBox->setAdditionalConstraints(m_constraints);
  }
}

}  // namespace caffeine
