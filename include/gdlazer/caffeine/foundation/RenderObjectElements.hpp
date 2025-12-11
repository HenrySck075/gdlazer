#pragma once

#include "Element.hpp"
#include "Widget.hpp"
#include "RenderObject.hpp"
#include <vector>

/* An Element that renders a CCNode (RenderObject) directly.
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

  void performRebuild() override;
  void update(Widget* newWidget) override;
  std::shared_ptr<Element> getAttachingRenderObjectChild() override;
  void visitChildren(std::function<void(std::shared_ptr<Element>)> visitor) override;

  std::shared_ptr<caffeine::RenderObject> createRenderObject() override;
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

  void performRebuild() override;
  void update(Widget* newWidget) override;
  void visitChildren(std::function<void(std::shared_ptr<Element>)> visitor) override;

  std::shared_ptr<caffeine::RenderObject> createRenderObject() override;
  virtual void insertRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) override;
  virtual void removeRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) override;

  virtual ~MultiChildRenderObjectElement() = default;
};
