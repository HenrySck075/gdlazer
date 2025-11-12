#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <Geode/cocos/include/cocos2d.h>
#include "shared_ptr_2.hpp"

class Widget;
class Element;
using ElementVisitor = std::function<void(shared_ptr_ctor<Element>)>;

class Element {
  std::shared_ptr<Element> m_parent;
  std::shared_ptr<Widget> m_widget;

public:
  Element(shared_ptr_ctor<Widget> widget) : m_widget(widget) {}

  /// Creates a new render object (aka cocos node)
  /// The default behavior if this is not overridden is to traverse into its child to find a render object and returns it (which can also be nullptr)
  virtual cocos2d::CCNode* createRenderObject();

  void mount(shared_ptr_ctor<Element> parent) {
    m_parent = parent;
  }


  /// given that there's only one child
  std::shared_ptr<Element> getAttachingRenderObjectChild() {
    std::shared_ptr<Element> next;
    visitChildren([&next](shared_ptr_ctor<Element> node){
      assert(!next);
      next = node;
    });
    return next;
  }

  /// Calls the argument for each child. Must be overridden by subclasses that
  /// support having children.
  ///
  /// There is no guaranteed order in which the children will be visited, though
  /// it should be consistent over time.
  ///
  /// Calling this during build is dangerous: the child list might still be
  /// being updated at that point, so the children might not be constructed yet,
  /// or might be old children that are going to be replaced. This method should
  /// only be called if it is provable that the children are available.
  
  virtual void visitChildren(ElementVisitor visitor) {}
};
