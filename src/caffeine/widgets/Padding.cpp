#include "gdlazer/caffeine/widgets/Padding.hpp"
#include "gdlazer/caffeine/rendering/object/RenderObject.hpp"
#include <memory>

namespace caffeine {

void RenderPadding::performLayout() {
  float horizontal = m_insets.horizontal();
  float vertical = m_insets.vertical();

  if (m_child) {
    BoxConstraints childConstraints = m_constraints.deflate(&m_insets);
    m_child->layout(childConstraints, true);

    m_size = m_constraints.constrain(Size(
      m_child->getSize().width + horizontal,
      m_child->getSize().height + vertical
    ));

    positionChild(m_insets.topLeft());
  } else {
    m_size = m_constraints.constrain(Size(horizontal, vertical));
  }
}

std::shared_ptr<RenderObject> Padding::createRenderObject() {
  return std::make_shared<RenderPadding>(m_insets, nullptr);
}
}  // namespace caffeine

