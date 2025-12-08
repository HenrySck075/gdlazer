#pragma once

#include "Element.hpp"
#include "Widget.hpp"
#include "utils/shared_ptr_2.hpp"

/* An Element that composes other Elements.

Rather than creating a RenderObject directly, a ComponentElement creates RenderObjects indirectly by creating other Elements.

Contrast with RenderObjectElement.
*/
class ComponentElement : public Element {
protected:
  std::shared_ptr<Element> m_child;

public:
  ComponentElement(shared_ptr_ctor<Widget> widget) : Element(widget) {};
  void performRebuild() override;
  virtual std::shared_ptr<Widget> build() = 0;
  std::shared_ptr<Element> getAttachingRenderObjectChild() override;
}; 


class StatelessElement : public ComponentElement {
protected:
  std::shared_ptr<StatelessWidget> m_widget;
public:
  StatelessElement(shared_ptr_ctor<StatelessWidget> widget) : ComponentElement(static_cast<Widget*>(widget.get())), m_widget(widget) {};
  std::shared_ptr<Widget> build() override {return m_widget->build(std::shared_ptr<BuildContext>(this));} 
};
