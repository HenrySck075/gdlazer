#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include "../../foundation/Key.hpp"
#include "gdlazer/caffeine/rendering/object/RenderObject.hpp"
#include <memory>
#include <vector>

namespace caffeine {
class Element;
class BuildContext;
class State;

class Widget : public cocos2d::CCObject {
  friend class Element;
private:
  RefNauseam<Key> m_key;
public:
  const RefNauseam<Key>& getKey() const { return m_key; }
  virtual RefNauseam<Element> createElement() = 0;
  static bool canUpdate(Widget* newWidget, Widget* oldWidget);

  virtual ~Widget() = default;
};



class StatelessWidget : public Widget {
public:
  virtual RefNauseam<Element> createElement();
  virtual Widget* build(RefNauseam<BuildContext> context) = 0;

  virtual ~StatelessWidget() = default;
};


class State;

class StatefulWidget : public Widget {
public:
  virtual RefNauseam<Element> createElement();
  virtual RefNauseam<State> createState() = 0;

  virtual ~StatefulWidget() = default;
};


class RenderObjectWidget : public Widget {
public:

  /// Creates the render object for this widget.
  /// Subclasses must override this to create their specific render object type.
  virtual RefNauseam<RenderObject> createRenderObject() = 0;
  
  /// Copies the configuration described by this RenderObjectWidget to the given RenderObject, which will be of the same type as returned by this object's createRenderObject.
  ///
  /// This method should not do anything to update the children of the render object. That should instead be handled by the method that overrides RenderObjectElement.update in the object rendered by this object's createElement method. See, for example, SingleChildRenderObjectElement::update.
  virtual void updateRenderObject(RefNauseam<BuildContext> context, RefNauseam<RenderObject> renderObject) {}

  virtual ~RenderObjectWidget() = default;
};


/// A superclass for RenderObjectWidgets that configure RenderObject subclasses that have no children.
class LeafRenderObjectWidget : public RenderObjectWidget {
public:
  virtual RefNauseam<Element> createElement();
};


class ProxyWidget : public Widget {
protected:
  RefNauseam<Widget> m_child;
public:
  // Accept raw Widget* and convert to shared_ptr internally
  // Enables: new MyProxyWidget(new ChildWidget())
  ProxyWidget(Widget* child) : m_child(RefNauseam<Widget>(child)) {}
  
  const Widget* getChild() const { return m_child.get(); }

  virtual ~ProxyWidget() = default;
};


class SingleChildRenderObjectWidget : public RenderObjectWidget {
protected:
  RefNauseam<Widget> m_child;
public:
  // Accept raw Widget* and convert to shared_ptr internally
  // Enables: new MyRenderWidget(new ChildWidget())
  SingleChildRenderObjectWidget(Widget* child) : m_child(RefNauseam<Widget>(child)) {}
  
  const Widget* getChild() const { return m_child.get(); }
  virtual RefNauseam<Element> createElement();

  virtual ~SingleChildRenderObjectWidget() = default;
};


class MultiChildRenderObjectWidget : public RenderObjectWidget {
protected:
  std::vector<RefNauseam<Widget>> m_children;
public:
  // Accept raw Widget pointers for convenience, convert to shared_ptr internally
  MultiChildRenderObjectWidget(std::vector<Widget*> children) {
    for (auto* child : children) {
      m_children.push_back(RefNauseam<Widget>(child));
    }
  }
  
  const std::vector<RefNauseam<Widget>>& getChildren() const { return m_children; }
  virtual RefNauseam<Element> createElement();

  virtual ~MultiChildRenderObjectWidget() = default;
};

}
