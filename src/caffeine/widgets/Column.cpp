#include "gdlazer/caffeine/widgets/Column.hpp"

namespace caffeine {

void RenderColumn::performLayout() {
  if (m_children.empty()) {
    m_size = m_constraints.biggest();
    return;
  }

  float totalHeight = 0;
  float maxWidth = 0;

  for (auto& child : m_children) {
    BoxConstraints childConstraints = BoxConstraints::loose(m_constraints.biggest());
    child->layout(childConstraints, true);

    totalHeight += child->getSize().height;
    maxWidth = std::max(maxWidth, child->getSize().width);
  }

  m_size = m_constraints.constrain(Size(maxWidth, totalHeight));

  float currentY = 0;
  for (auto& child : m_children) {
    float childX = (maxWidth - child->getSize().width) * 0.5f;
    positionChild(child, Offset(childX, currentY));
    currentY += child->getSize().height;
  }
}

void RenderColumn::paint(SkCanvas* canvas) {
  for (auto& child : m_children) {
    child->paint(canvas);
  }
}

std::shared_ptr<RenderObject> Column::createRenderObject() {
  return std::make_shared<RenderColumn>();
}


}  // namespace caffeine

