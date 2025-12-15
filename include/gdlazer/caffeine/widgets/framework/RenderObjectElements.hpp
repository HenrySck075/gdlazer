#pragma once

#include "Element.hpp"
#include "Widget.hpp"
#include "../../rendering/object/RenderObject.hpp"
#include <vector>

namespace caffeine {
/// The elements that renders a direct RenderObject and manages its state, with a Widget as its configuration.
class RenderObjectElement : public Element {
private:
  std::shared_ptr<RenderObject> m_renderObject;
  std::shared_ptr<RenderObjectElement> findAncestorRenderObjectElement();
  std::shared_ptr<RenderObjectElement> m_ancestorRenderObjectElement;

public:
  RenderObjectElement(Widget* widget) : Element(widget) {}
  virtual ~RenderObjectElement() = default;
  
  void mount(std::shared_ptr<Element> parent, void* slot) override;
  /// Attach this element's render object to the render tree.
  void attachRenderObject(void* newSlot) final override;
  /// Detach this element's render object from the render tree.
  void detachRenderObject() final override;

  virtual void insertRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) = 0; 
  virtual void removeRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) = 0;

  std::shared_ptr<caffeine::RenderObject> getRenderObject() override;

  void performRebuild() override {
    std::static_pointer_cast<RenderObjectWidget>(m_widget)
  };
};
/* An Element that renders a RenderObject directly.
 *
 * Each RenderObjectWidget has a corresponding RenderObjectElement.
 * This is the base class for elements that manage render objects.
 */

class SingleChildRenderObjectElement : public RenderObjectElement {
protected:
  std::shared_ptr<caffeine::RenderBox> m_renderBox;
  std::shared_ptr<Element> m_child;

public:
  SingleChildRenderObjectElement(SingleChildRenderObjectWidget* widget)
    : RenderObjectElement(widget) {}

  void mount(std::shared_ptr<Element> parent, void *slot) override;
  void performRebuild() override;
  void update(Widget* newWidget) override;
  std::shared_ptr<Element> getAttachingRenderObjectChild() override;
  void visitChildren(std::function<void(std::shared_ptr<Element>)> visitor) override;

  virtual void insertRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) override {}
  virtual void removeRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) override {}

  virtual ~SingleChildRenderObjectElement() = default;
};


class MultiChildRenderObjectElement : public RenderObjectElement {
protected:
  std::vector<std::shared_ptr<Element>> m_children;

public:
  MultiChildRenderObjectElement(MultiChildRenderObjectWidget* widget)
    : RenderObjectElement(widget) {}

  void mount(std::shared_ptr<Element> parent, void *slot) override;
  void performRebuild() override;
  void update(Widget* newWidget) override;
  void visitChildren(std::function<void(std::shared_ptr<Element>)> visitor) override;

  virtual void insertRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) override;
  virtual void removeRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) override;

  virtual ~MultiChildRenderObjectElement() = default;
};

}
