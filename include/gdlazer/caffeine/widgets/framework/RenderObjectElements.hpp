#pragma once

#include "Element.hpp"
#include "Widget.hpp"
#include "../../rendering/object/RenderObject.hpp"
#include <vector>

#include <gdlazer/caffeine/foundation/utils/Ref.hpp>
namespace caffeine {
/* An Element that renders a RenderObject directly.
 *
 * Each RenderObjectWidget has a corresponding RenderObjectElement.
 * This is the base class for elements that manage render objects.
 */

class RenderObjectElement : public Element {
protected:
  RefNauseam<RenderObject> m_renderObject;
  RefNauseam<RenderObjectElement> findAncestorRenderObjectElement();
  RefNauseam<RenderObjectElement> m_ancestorRenderObjectElement;

public:
  RenderObjectElement(RenderObjectWidget* widget) : Element(widget) {}
  virtual ~RenderObjectElement() = default;
  
  void mount(RefNauseam<Element> parent, void* slot) override;
  /// Attach this element's render object to the render tree.
  void attachRenderObject(void* newSlot) final override;
  /// Detach this element's render object from the render tree.
  void detachRenderObject() final override;

  virtual void insertRenderObjectChild(RefNauseam<caffeine::RenderObject> child) = 0; 
  virtual void removeRenderObjectChild(RefNauseam<caffeine::RenderObject> child) = 0;

  RefNauseam<caffeine::RenderObject> getRenderObject() override;

  void performRebuild() override {
    geode::log::debug("c");
    static_cast<RenderObjectWidget*>(m_widget.get())->updateRenderObject(this, m_renderObject);
    Element::performRebuild();
  };
  void update(Widget* newWidget) override {
    Element::update(newWidget);
    performRebuild();
  };
};

class LeafRenderObjectElement : public RenderObjectElement {
public:
  LeafRenderObjectElement(LeafRenderObjectWidget* widget)
    : RenderObjectElement(widget) {}
  void insertRenderObjectChild(RefNauseam<caffeine::RenderObject> child) final override {}; 
  void removeRenderObjectChild(RefNauseam<caffeine::RenderObject> child) final override {};
};

class SingleChildRenderObjectElement : public RenderObjectElement {
protected:
  RefNauseam<Element> m_child;

public:
  SingleChildRenderObjectElement(SingleChildRenderObjectWidget* widget)
    : RenderObjectElement(widget) {}

  void mount(RefNauseam<Element> parent, void *slot) override;
  //void performRebuild() override;
  void update(Widget* newWidget) override;
  RefNauseam<Element> getAttachingRenderObjectChild() override;
  void visitChildren(std::function<void(RefNauseam<Element>)> visitor) override;

  virtual void insertRenderObjectChild(RefNauseam<caffeine::RenderObject> child) override {
    auto renderObject = dynamic_cast<RenderObjectWithChildMixin*>(m_renderObject.get());
    if (renderObject) renderObject->setChild(child);
  }
  virtual void removeRenderObjectChild(RefNauseam<caffeine::RenderObject> child) override {
    auto renderObject = dynamic_cast<RenderObjectWithChildMixin*>(m_renderObject.get());
    if (renderObject) {
      assert(renderObject->getChild() == child);
      renderObject->setChild(nullptr);
    }
  }

  virtual ~SingleChildRenderObjectElement() = default;
};


class MultiChildRenderObjectElement : public RenderObjectElement {
protected:
  std::vector<RefNauseam<Element>> m_children;

public:
  MultiChildRenderObjectElement(MultiChildRenderObjectWidget* widget)
    : RenderObjectElement(widget) {}

  void mount(RefNauseam<Element> parent, void *slot) override;
  void performRebuild() override;
  void update(Widget* newWidget) override;
  void visitChildren(std::function<void(RefNauseam<Element>)> visitor) override;

  virtual void insertRenderObjectChild(RefNauseam<caffeine::RenderObject> child) override;
  virtual void removeRenderObjectChild(RefNauseam<caffeine::RenderObject> child) override;

  virtual ~MultiChildRenderObjectElement() = default;
};

}
