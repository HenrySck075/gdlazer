#include "gdlazer/caffeine/rendering/proxy_box/aligned_box.hpp"
#include <cmath>

namespace caffeine {

void RenderAligningShiftedBox::performLayout() {
  if (!m_child) {
    m_size = m_constraints.constrain(Size(0, 0));
    return;
  }

  // Layout child with loose constraints
  Size childSize = ChildLayoutHelper::layoutChild(
    std::dynamic_pointer_cast<RenderBox>(m_child),
    BoxConstraints::loose(m_constraints.biggest())
  );

  // Apply alignment to calculate position
  Offset alignmentOffset = m_alignment->alongSize(
    Size(m_constraints.biggest().width - childSize.width,
         m_constraints.biggest().height - childSize.height)
  );

  // Position child at the aligned offset
  positionChild(alignmentOffset);

  // Size ourselves to fill constraints
  m_size = m_constraints.constrain(m_constraints.biggest());
}

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
    // alignChild()
  } else {
    m_size = m_constraints.constrain({
      shrinkWrapWidth ? 0 : INFINITY,
      shrinkWrapHeight ? 0 : INFINITY,
    }); 
  }
}

}  // namespace caffeine
