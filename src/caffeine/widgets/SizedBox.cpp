#include "gdlazer/caffeine/widgets/SizedBox.hpp"
#include <memory>

namespace caffeine {

void RenderSizedBox::performLayout() {
  m_size = m_constraints.constrain(m_size);

  if (m_child) {
    BoxConstraints childConstraints = BoxConstraints::tight(m_size);
    layoutChild(childConstraints, false);
    positionChild(Offset(0, 0));
  }
}

std::shared_ptr<RenderObject> SizedBox::createRenderObject() {
  return std::make_shared<RenderSizedBox>(m_size);
}


}  // namespace caffeine

