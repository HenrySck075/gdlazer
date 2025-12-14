#pragma once

#include <memory>
#include "../foundation/RenderObject.hpp"
#include "../foundation/Widget.hpp"
#include "../foundation/Element.hpp"

namespace caffeine {

// ============================================================================
// Padding: Adds padding around child
// ============================================================================

class RenderPadding : public SingleChildRenderBox {
private:
  float m_left = 0, m_top = 0, m_right = 0, m_bottom = 0;

public:
  RenderPadding(float left, float top, float right, float bottom)
    : m_left(left), m_top(top), m_right(right), m_bottom(bottom) {}

  RenderPadding(float padding)
    : m_left(padding), m_top(padding), m_right(padding), m_bottom(padding) {}

  void performLayout() override {
    float horizontal = m_left + m_right;
    float vertical = m_top + m_bottom;

    if (m_child) {
      BoxConstraints childConstraints = m_constraints.deflate(m_left, m_top, m_right, m_bottom);
      layoutChild(childConstraints, true);

      m_size = m_constraints.constrain(Size(
        m_child->getSize().width + horizontal,
        m_child->getSize().height + vertical
      ));

      // Position child with padding offset
      positionChild(Offset(m_left, m_top));
    } else {
      m_size = m_constraints.constrain(Size(horizontal, vertical));
    }
  }

  void paint(SkCanvas* canvas) override {
    if (m_child) {
      m_child->paint(canvas);
    }
  }
};

// ============================================================================
// Padding Widget & Element
// ============================================================================

class PaddingElement;

class Padding : public SingleChildRenderObjectWidget {
private:
  float m_left = 0, m_top = 0, m_right = 0, m_bottom = 0;

public:
  Padding(float left, float top, float right, float bottom, Widget* child = nullptr)
    : SingleChildRenderObjectWidget(child), m_left(left), m_top(top), m_right(right), m_bottom(bottom) {}

  Padding(float padding, Widget* child = nullptr)
    : SingleChildRenderObjectWidget(child), m_left(padding), m_top(padding), m_right(padding), m_bottom(padding) {}

  float getLeft() const { return m_left; }
  float getTop() const { return m_top; }
  float getRight() const { return m_right; }
  float getBottom() const { return m_bottom; }

  std::shared_ptr<Element> createElement() override;
};

class PaddingElement : public RenderObjectElement {
private:
  Padding* m_widget;

protected:
  std::shared_ptr<RenderObject> createRenderObject() override {
    return std::make_shared<RenderPadding>(
      m_widget->getLeft(), m_widget->getTop(), m_widget->getRight(), m_widget->getBottom()
    );
  }

  void insertRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderPadding = std::dynamic_pointer_cast<RenderPadding>(getRenderObject());
    auto renderBox = std::dynamic_pointer_cast<RenderBox>(child);
    if (renderPadding && renderBox) {
      renderPadding->setChild(renderBox);
    }
  }

  void removeRenderObjectChild(std::shared_ptr<RenderObject> child) override {
    auto renderPadding = std::dynamic_pointer_cast<RenderPadding>(getRenderObject());
    if (renderPadding) {
      renderPadding->setChild(nullptr);
    }
  }

public:
  PaddingElement(Widget* widget) : RenderObjectElement(widget), m_widget(static_cast<Padding*>(widget)) {}
  virtual ~PaddingElement() = default;
};

}  // namespace caffeine
