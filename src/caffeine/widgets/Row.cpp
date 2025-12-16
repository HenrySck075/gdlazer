#include "gdlazer/caffeine/widgets/Row.hpp"

namespace caffeine {

void RenderRow::performLayout() {
  if (m_children.empty()) {
    m_size = m_constraints.biggest();
    return;
  }

  float totalWidth = 0;
  float maxHeight = 0;

  for (auto& child : m_children) {
    BoxConstraints childConstraints = BoxConstraints::loose(m_constraints.biggest());
    child->layout(childConstraints, true);

    totalWidth += child->getSize().width;
    maxHeight = std::max(maxHeight, child->getSize().height);
  }

  m_size = m_constraints.constrain(Size(totalWidth, maxHeight));

  float currentX = 0;
  for (auto& child : m_children) {
    float childY = (maxHeight - child->getSize().height) * 0.5f;
    ChildLayoutHelper::positionChild(child, Offset(currentX, childY));
    currentX += child->getSize().width;
  }
}

void RenderRow::paint(SkCanvas* canvas) {
  for (auto& child : m_children) {
    child->paint(canvas);
  }
}

std::shared_ptr<RenderObject> Row::createRenderObject() {
  return std::make_shared<RenderRow>();
}
}  // namespace caffeine

