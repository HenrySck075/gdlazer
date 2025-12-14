#pragma once

#include <memory>
#include "../foundation/RenderObject.hpp"
#include "../foundation/Widget.hpp"
#include "../foundation/Element.hpp"

namespace caffeine {

// ============================================================================
// Column: Lays out children vertically
// ============================================================================

class RenderColumn : public MultiChildRenderBox {
public:
  void performLayout() override {
    if (m_children.empty()) {
      m_size = m_constraints.biggest();
      return;
    }

    float totalHeight = 0;
    float maxWidth = 0;

    // First pass: layout all children
    for (auto& child : m_children) {
      BoxConstraints childConstraints = BoxConstraints::loose(m_constraints.maxWidth, m_constraints.maxHeight);
      child->layout(childConstraints, true);

      totalHeight += child->getSize().height;
      maxWidth = std::max(maxWidth, child->getSize().width);
    }

    // Constrain our size
    m_size = m_constraints.constrain(Size(maxWidth, totalHeight));

    // Second pass: position children
    float currentY = 0;
    for (auto& child : m_children) {
      float childX = (maxWidth - child->getSize().width) * 0.5f;  // Center horizontally
      positionChild(child, Offset(childX, currentY));
      currentY += child->getSize().height;
    }
  }

  void paint(SkCanvas* canvas) override {
    for (auto& child : m_children) {
      child->paint(canvas);
    }
  }
};

// ============================================================================
// Column Widget & Element
// ============================================================================

class ColumnElement;

class Column : public MultiChildRenderObjectWidget {
public:
  Column(std::vector<Widget*> children = {}) : MultiChildRenderObjectWidget(children) {}

  std::shared_ptr<Element> createElement() override;
};

class ColumnElement : public RenderObjectElement {
private:
  Column* m_widget;

protected:
  std::shared_ptr<RenderObject> createRenderObject() override {
    return std::make_shared<RenderColumn>();
  }

  void insertRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderColumn = std::dynamic_pointer_cast<RenderColumn>(getRenderObject());
    auto renderBox = std::dynamic_pointer_cast<RenderBox>(child);
    if (renderColumn && renderBox) {
      renderColumn->addChild(renderBox);
    }
  }

  void removeRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderColumn = std::dynamic_pointer_cast<RenderColumn>(getRenderObject());
    auto renderBox = std::dynamic_pointer_cast<RenderBox>(child);
    if (renderColumn && renderBox) {
      renderColumn->removeChild(renderBox);
    }
  }

public:
  ColumnElement(Widget* widget) : RenderObjectElement(widget), m_widget(static_cast<Column*>(widget)) {}
  virtual ~ColumnElement() = default;
};

}  // namespace caffeine
