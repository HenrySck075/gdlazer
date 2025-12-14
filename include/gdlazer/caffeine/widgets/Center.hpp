#pragma once

#include <memory>
#include "../foundation/RenderObject.hpp"
#include "../foundation/Widget.hpp"
#include "../foundation/Element.hpp"

namespace caffeine {

// ============================================================================
// Center: Centers child within available space
// ============================================================================

class RenderCenter : public SingleChildRenderBox {
public:
  void performLayout() override {
    if (m_child) {
      layoutChild(m_constraints, true);
      m_size = m_constraints.constrain(m_child->getSize());

      // Center the child
      Offset offset(
        (m_size.width - m_child->getSize().width) * 0.5f,
        (m_size.height - m_child->getSize().height) * 0.5f
      );
      positionChild(offset);
    } else {
      m_size = m_constraints.biggest();
    }
  }

  void paint(SkCanvas* canvas) override {
    if (m_child) {
      m_child->paint(canvas);
    }
  }
};

// ============================================================================
// Center Widget & Element
// ============================================================================

class CenterElement;

class Center : public SingleChildRenderObjectWidget {
public:
  Center(Widget* child = nullptr) : SingleChildRenderObjectWidget(child) {}

  std::shared_ptr<Element> createElement() override;
};

class CenterElement : public RenderObjectElement {
private:
  Center* m_widget;

protected:
  std::shared_ptr<RenderObject> createRenderObject() override {
    return std::make_shared<RenderCenter>();
  }

  void insertRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderCenter = std::dynamic_pointer_cast<RenderCenter>(getRenderObject());
    auto renderBox = std::dynamic_pointer_cast<RenderBox>(child);
    if (renderCenter && renderBox) {
      renderCenter->setChild(renderBox);
    }
  }

  void removeRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderCenter = std::dynamic_pointer_cast<RenderCenter>(getRenderObject());
    if (renderCenter) {
      renderCenter->setChild(nullptr);
    }
  }

public:
  CenterElement(Widget* widget) : RenderObjectElement(widget), m_widget(static_cast<Center*>(widget)) {}
  virtual ~CenterElement() = default;
};

}  // namespace caffeine
