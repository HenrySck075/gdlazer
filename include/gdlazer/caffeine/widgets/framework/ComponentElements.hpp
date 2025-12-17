#pragma once

#include "Element.hpp"
#include "Widget.hpp"

namespace caffeine {
/* An Element that composes other Elements.

Rather than creating a RenderObject directly, a ComponentElement creates RenderObjects indirectly by creating other Elements.

Contrast with RenderObjectElement.
*/
class ComponentElement : public Element {
protected:
  std::shared_ptr<Element> m_child;

public:
  ComponentElement(Widget* widget) : Element(widget) {};
  void performRebuild() override;
  virtual Widget* build() = 0;
  std::shared_ptr<Element> getAttachingRenderObjectChild() override;
}; 


class StatelessElement : public ComponentElement {
public:
  StatelessElement(StatelessWidget* widget) 
    : ComponentElement(widget) {}
  Widget* build() override {
    auto widget = std::static_pointer_cast<StatelessWidget>(m_widget);
    return widget->build(std::shared_ptr<BuildContext>(this));
  } 
  void update(Widget* newWidget) override;
};


class StatefulElement : public ComponentElement {
protected:
  std::shared_ptr<State> m_state;
  std::shared_ptr<StatefulWidget> m_widget;

public:
  StatefulElement(StatefulWidget *widget);

  void setStateElement(std::shared_ptr<StatefulElement> self);

  virtual void mount(std::shared_ptr<Element> parent, void* slot) override;
  virtual void performRebuild() override;
  Widget* build() override;
  virtual void update(Widget* newWidget) override;
};


class ProxyElement : public ComponentElement {
protected:
  std::shared_ptr<ProxyWidget> m_widget;

public:
  ProxyElement(ProxyWidget* widget) 
    : ComponentElement(widget), m_widget(std::shared_ptr<ProxyWidget>(widget)) {}

  Widget* build() override {
    return const_cast<Widget*>(m_widget->getChild());
  };
  void update(Widget* newWidget) override;
};
}
