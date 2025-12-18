#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include "Element.hpp"
#include "Widget.hpp"

namespace caffeine {
/* An Element that composes other Elements.

Rather than creating a RenderObject directly, a ComponentElement creates RenderObjects indirectly by creating other Elements.

Contrast with RenderObjectElement.
*/
class ComponentElement : public Element {
protected:
  RefNauseam<Element> m_child;

public:
  ComponentElement(Widget* widget) : Element(widget) {};
  void performRebuild() override;
  virtual Widget* build() = 0;
  RefNauseam<Element> getAttachingRenderObjectChild() override;
}; 


class StatelessElement : public ComponentElement {
public:
  StatelessElement(StatelessWidget* widget) 
    : ComponentElement(widget) {}
  Widget* build() override {
    auto widget = dynamic_cast<StatelessWidget*>(m_widget.get());
    return widget->build(this);
  } 
  void update(Widget* newWidget) override;
};


class StatefulElement : public ComponentElement {
protected:
  RefNauseam<State> m_state;
  RefNauseam<StatefulWidget> m_widget;

public:
  StatefulElement(StatefulWidget* widget);

  void setStateElement(StatefulElement* self);

  virtual void mount(RefNauseam<Element> parent, void* slot) override;
  //virtual void performRebuild() override;
  Widget* build() override;
  virtual void update(Widget* newWidget) override;
};


class ProxyElement : public ComponentElement {
protected:
  RefNauseam<ProxyWidget> m_widget;

public:
  ProxyElement(ProxyWidget* widget) 
    : ComponentElement(widget), m_widget(widget) {}

  Widget* build() override {
    return const_cast<Widget*>(m_widget->getChild());
  };
  void update(Widget* newWidget) override;
};
}
