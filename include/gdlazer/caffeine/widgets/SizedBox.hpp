#pragma once

#include <memory>
#include "../foundation/RenderObject.hpp"
#include "../foundation/Widget.hpp"
#include "../foundation/Element.hpp"

namespace caffeine {

// ============================================================================
// SizedBox: Fixed size container
// ============================================================================

class RenderSizedBox : public SingleChildRenderBox {
private:
  float m_width = 0.0f;
  float m_height = 0.0f;

public:
  RenderSizedBox(float width, float height) : m_width(width), m_height(height) {}

  void performLayout() override {
    m_size = m_constraints.constrain(Size(m_width, m_height));

    if (m_child) {
      BoxConstraints childConstraints = BoxConstraints::tight(m_width, m_height);
      layoutChild(childConstraints, false);
      // Position child at (0, 0)
      positionChild(Offset(0, 0));
    }
  }

  void paint(SkCanvas* canvas) override {
    if (m_child) {
      m_child->paint(canvas);
    }
  }
};

// ============================================================================
// SizedBox Widget & Element
// ============================================================================

class SizedBoxElement;

class SizedBox : public SingleChildRenderObjectWidget {
private:
  float m_width = 0.0f;
  float m_height = 0.0f;

public:
  SizedBox(float width, float height, Widget* child = nullptr)
    : SingleChildRenderObjectWidget(child), m_width(width), m_height(height) {}

  float getWidth() const { return m_width; }
  float getHeight() const { return m_height; }

  std::shared_ptr<Element> createElement() override;
};

class SizedBoxElement : public RenderObjectElement {
private:
  SizedBox* m_widget;

protected:
  std::shared_ptr<RenderObject> createRenderObject() override {
    return std::make_shared<RenderSizedBox>(m_widget->getWidth(), m_widget->getHeight());
  }

  void insertRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderSizedBox = std::dynamic_pointer_cast<RenderSizedBox>(getRenderObject());
    auto renderBox = std::dynamic_pointer_cast<RenderBox>(child);
    if (renderSizedBox && renderBox) {
      renderSizedBox->setChild(renderBox);
    }
  }

  void removeRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderSizedBox = std::dynamic_pointer_cast<RenderSizedBox>(getRenderObject());
    if (renderSizedBox) {
      renderSizedBox->setChild(nullptr);
    }
  }

public:
  SizedBoxElement(Widget* widget) : RenderObjectElement(widget), m_widget(static_cast<SizedBox*>(widget)) {}
  virtual ~SizedBoxElement() = default;
};

}  // namespace caffeine
