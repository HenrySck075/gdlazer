#include "gdlazer/caffeine/rendering/proxy_box/aligned_box.hpp"
#include <cmath>

namespace caffeine {

void RenderPositionedBox::performLayout() {
  bool shrinkWrapWidth = m_widthFactor.has_value() || m_constraints.maxWidth != INFINITY;
  bool shrinkWrapHeight = m_heightFactor.has_value() || m_constraints.maxHeight != INFINITY;

  if (m_child) {
    m_child->layout(m_constraints.loosen());
    auto childSize = m_child->getSize();
    m_size = m_constraints.constrain({
      shrinkWrapWidth ? childSize.width * m_widthFactor.value_or(1) : INFINITY,
      shrinkWrapHeight ? childSize.height * m_heightFactor.value_or(1) : INFINITY,
    }); 
    alignChild();
  } else {
    m_size = m_constraints.constrain({
      shrinkWrapWidth ? 0 : INFINITY,
      shrinkWrapHeight ? 0 : INFINITY,
    }); 
  }
}

}  // namespace caffeine
