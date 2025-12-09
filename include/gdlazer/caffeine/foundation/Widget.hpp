#pragma once

#include "Key.hpp"
#include "utils/readonly.hpp"
#include "utils/shared_ptr_2.hpp"
#include <memory>
#include <vector>
class Element;
class BuildContext;
class State;

class Widget {
public:
  readonly<std::shared_ptr<Key>, Element> m_key;
  virtual std::shared_ptr<Element> createElement() {return nullptr;};
  static bool canUpdate(Widget* newWidget, Widget* oldWidget);
};





class StatelessWidget : public Widget {
public:
  virtual std::shared_ptr<Element> createElement();
  virtual Widget* build(std::shared_ptr<BuildContext> context) = 0;
};


class State;

class StatefulWidget : public Widget {
public:
  virtual std::shared_ptr<Element> createElement();
  virtual std::shared_ptr<State> createState() = 0;
};


class RenderObjectWidget : public Widget {
public:
  virtual std::shared_ptr<Element> createElement();
};


class ProxyWidget : public Widget {
protected:
  std::shared_ptr<Widget> m_child;
public:
  ProxyWidget(shared_ptr_ctor<Widget> child) : m_child(child) {}
  Widget* getChild() const { return m_child.get(); }
  virtual std::shared_ptr<Element> createElement() = 0;
};


class SingleChildRenderObjectWidget : public RenderObjectWidget {
protected:
  std::shared_ptr<Widget> m_child;
public:
  SingleChildRenderObjectWidget(shared_ptr_ctor<Widget> child) : m_child(child) {}
  Widget* getChild() const { return m_child.get(); }
  virtual std::shared_ptr<Element> createElement();
};


class MultiChildRenderObjectWidget : public RenderObjectWidget {
protected:
  std::vector<std::shared_ptr<Widget>> m_children;
public:
  MultiChildRenderObjectWidget(std::vector<std::shared_ptr<Widget>> children) : m_children(children) {}
  const std::vector<std::shared_ptr<Widget>>& getChildren() const { return m_children; }
  virtual std::shared_ptr<Element> createElement();
};
