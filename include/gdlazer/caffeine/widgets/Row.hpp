#pragma once

#include <memory>
#include "../foundation/RenderObject.hpp"
#include "../foundation/Widget.hpp"
#include "../foundation/Element.hpp"

namespace caffeine {

// ============================================================================
// Row: Lays out children horizontally
// ============================================================================

class RenderRow : public MultiChildRenderBox {
public:
  // MainAxisAlignment and CrossAxisAlignment would go here in full flutter
  // For now, keeping it simple

  void performLayout() override {
    if (m_children.empty()) {
      m_size = m_constraints.biggest();
      return;
    }

    float totalWidth = 0;
    float maxHeight = 0;

    // First pass: layout all children with max width available
    for (auto& child : m_children) {
      BoxConstraints childConstraints = BoxConstraints::loose(m_constraints.maxWidth, m_constraints.maxHeight);
      child->layout(childConstraints, true);

      totalWidth += child->getSize().width;
      maxHeight = std::max(maxHeight, child->getSize().height);
    }

    // Constrain our size
    m_size = m_constraints.constrain(Size(totalWidth, maxHeight));

    // Second pass: position children
    float currentX = 0;
    for (auto& child : m_children) {
      float childY = (maxHeight - child->getSize().height) * 0.5f;  // Center vertically
      positionChild(child, Offset(currentX, childY));
      currentX += child->getSize().width;
    }
  }

  void paint(SkCanvas* canvas) override {
    for (auto& child : m_children) {
      child->paint(canvas);
    }
  }
};

// ============================================================================
// Row Widget & Element
// ============================================================================

class RowElement;

class Row : public MultiChildRenderObjectWidget {
public:
  Row(std::vector<Widget*> children = {}) : MultiChildRenderObjectWidget(children) {}

  std::shared_ptr<Element> createElement() override;
};

class RowElement : public RenderObjectElement {
private:
  Row* m_widget;

protected:
  std::shared_ptr<RenderObject> createRenderObject() override {
    return std::make_shared<RenderRow>();
  }

  void insertRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderRow = std::dynamic_pointer_cast<RenderRow>(getRenderObject());
    auto renderBox = std::dynamic_pointer_cast<RenderBox>(child);
    if (renderRow && renderBox) {
      renderRow->addChild(renderBox);
    }
  }

  void removeRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderRow = std::dynamic_pointer_cast<RenderRow>(getRenderObject());
    auto renderBox = std::dynamic_pointer_cast<RenderBox>(child);
    if (renderRow && renderBox) {
      renderRow->removeChild(renderBox);
    }
  }

public:
  RowElement(Widget* widget) : RenderObjectElement(widget), m_widget(static_cast<Row*>(widget)) {}
  virtual ~RowElement() = default;
};

}  // namespace caffeine
