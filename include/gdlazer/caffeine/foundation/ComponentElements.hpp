#pragma once

#include "Element.hpp"
#include "Widget.hpp"
#include "utils/shared_ptr_2.hpp"
#include <vector>

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
protected:
  std::shared_ptr<StatelessWidget> m_widget;
public:
  StatelessElement(StatelessWidget* widget) 
    : ComponentElement(widget), m_widget(std::shared_ptr<StatelessWidget>(widget)) {}
  Widget* build() override {return m_widget->build(std::shared_ptr<BuildContext>(this));} 
  void update(Widget* newWidget) override;
};


class StatefulElement;

class State {
  friend class StatefulElement;
protected:
  std::shared_ptr<StatefulElement> m_element;
public:
  virtual void initState() {}
  virtual void didUpdateWidget(std::shared_ptr<StatefulWidget> oldWidget) {}
  virtual void dispose() {}
  virtual Widget* build(std::shared_ptr<BuildContext> context) = 0;
  void setState(std::function<void()> fn);
};


class StatefulElement : public ComponentElement {
protected:
  std::shared_ptr<State> m_state;
  std::shared_ptr<StatefulWidget> m_widget;

public:
  StatefulElement(StatefulWidget* widget) 
    : ComponentElement(widget), m_widget(std::shared_ptr<StatefulWidget>(widget)) {
    m_state = widget->createState();
  }

  void setStateElement(std::shared_ptr<StatefulElement> self) {
    m_state->m_element = self;
  }

  virtual void mount(std::shared_ptr<Element> parent, void* slot);
  virtual void performRebuild() override;
  Widget* build() override {return m_state->build(std::shared_ptr<BuildContext>(this));}
  virtual void update(Widget* newWidget);
};


class ProxyElement : public ComponentElement {
protected:
  std::shared_ptr<ProxyWidget> m_widget;

public:
  ProxyElement(ProxyWidget* widget) 
    : ComponentElement(widget), m_widget(std::shared_ptr<ProxyWidget>(widget)) {}

  Widget* build() override {return m_widget->getChild();};
  void update(Widget* newWidget) override;
};
