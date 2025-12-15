#include "gdlazer/caffeine/widgets/Center.hpp"

namespace caffeine {

void RenderCenter::performLayout() {
  if (m_child) {
    layoutChild(m_constraints, true);
    m_size = m_constraints.constrain(m_child->getSize());

    Offset offset(
      (m_size.width - m_child->getSize().width) * 0.5f,
      (m_size.height - m_child->getSize().height) * 0.5f
    );
    positionChild(offset);
  } else {
    m_size = m_constraints.biggest();
  }
}


std::shared_ptr<RenderObject> Center::createRenderObject() {
  return std::make_shared<RenderCenter>();
}

}  // namespace caffeine

