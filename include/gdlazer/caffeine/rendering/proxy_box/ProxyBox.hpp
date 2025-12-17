#pragma once

/// ProxyBox and other abstract subclasses probably

#include "gdlazer/caffeine/rendering/object/RenderObject.hpp"
namespace caffeine {
  /// Implementation of RenderProxyBox
  class RenderProxyBoxMixin : public RenderObjectWithChildMixin {
    void performLayout() override {
      m_size = m_child != nullptr ? ({m_child->layout(m_constraints, true); m_child->getSize();}) : computeSizeForNoChild(m_constraints);
    }
    Size computeSizeForNoChild(BoxConstraints& constraints) {
      return constraints.smallest();
    }
  public:
    void paint(SkCanvas* canvas) override {
      if (m_child) {
        m_child->paint(canvas);
      }
    }
  };
  /// A base class for render boxes that resemble their children.
  ///
  /// A proxy box has a single child and mimics all the properties of that
  /// child by calling through to the child for each function in the render box
  /// protocol. For example, a proxy box determines its size by asking its child
  /// to layout with the same constraints and then matching the size.
  ///
  /// A proxy box isn't useful on its own because you might as well just replace
  /// the proxy box with its child. However, RenderProxyBox is a useful base class
  /// for render objects that wish to mimic most, but not all, of the properties
  /// of their child.
  class RenderProxyBox : public RenderBox, public RenderProxyBoxMixin {
  public:
    RenderProxyBox(std::shared_ptr<RenderBox> child = nullptr) {m_child = child;}
  };

}
