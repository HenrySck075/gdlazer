#pragma once

#include "Key.hpp"
#include "utils/readonly.hpp"
#include <memory>
#include <vector>
class Element;
class BuildContext;
class State;

class Widget : public std::enable_shared_from_this<Widget> {
public:
  readonly<std::shared_ptr<Key>, Element> m_key;
  virtual std::shared_ptr<Element> createElement() {return nullptr;};
  static bool canUpdate(Widget* newWidget, Widget* oldWidget);

  virtual ~Widget() = default;
};



class StatelessWidget : public Widget {
public:
  virtual std::shared_ptr<Element> createElement();
  virtual Widget* build(std::shared_ptr<BuildContext> context) = 0;

  virtual ~StatelessWidget() = default;
};


class State;

class StatefulWidget : public Widget {
public:
  virtual std::shared_ptr<Element> createElement();
  virtual std::shared_ptr<State> createState() = 0;

  virtual ~StatefulWidget() = default;
};


class RenderObjectWidget : public Widget {
public:
  virtual std::shared_ptr<Element> createElement();

  virtual ~RenderObjectWidget() = default;
};


class ProxyWidget : public Widget {
protected:
  std::shared_ptr<Widget> m_child;
public:
  // Accept raw Widget* and convert to shared_ptr internally
  // Enables: new MyProxyWidget(new ChildWidget())
  ProxyWidget(Widget* child) : m_child(std::shared_ptr<Widget>(child)) {}
  
  const Widget* getChild() const { return m_child.get(); }
  virtual std::shared_ptr<Element> createElement() = 0;

  virtual ~ProxyWidget() = default;
};


class SingleChildRenderObjectWidget : public RenderObjectWidget {
protected:
  std::shared_ptr<Widget> m_child;
public:
  // Accept raw Widget* and convert to shared_ptr internally
  // Enables: new MyRenderWidget(new ChildWidget())
  SingleChildRenderObjectWidget(Widget* child) : m_child(std::shared_ptr<Widget>(child)) {}
  
  const Widget* getChild() const { return m_child.get(); }
  virtual std::shared_ptr<Element> createElement();

  virtual ~SingleChildRenderObjectWidget() = default;
};


class MultiChildRenderObjectWidget : public RenderObjectWidget {
protected:
  std::vector<std::shared_ptr<Widget>> m_children;
public:
  // Accept raw Widget pointers for convenience, convert to shared_ptr internally
  MultiChildRenderObjectWidget(std::vector<Widget*> children) {
    for (auto* child : children) {
      m_children.push_back(std::shared_ptr<Widget>(child));
    }
  }
  
  const std::vector<std::shared_ptr<Widget>>& getChildren() const { return m_children; }
  virtual std::shared_ptr<Element> createElement();

  virtual ~MultiChildRenderObjectWidget() = default;
};
