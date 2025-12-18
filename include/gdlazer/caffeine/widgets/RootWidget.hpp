#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include "gdlazer/caffeine/rendering/proxy_box/ProxyBox.hpp"
#include "gdlazer/caffeine/widgets/framework/Element.hpp"
#include "gdlazer/caffeine/widgets/framework/RenderObjectElements.hpp"
#include "gdlazer/caffeine/widgets/framework/Widget.hpp"

namespace caffeine {
class RootElement;
class BuildOwner;

/// A widget for the root of the widget tree.
/// Wraps the application's root widget and bootstraps the element tree.
class RootWidget : public SingleChildRenderObjectWidget {
public:
  // Accept raw Widget* and convert to shared_ptr internally
  RootWidget(Widget* child = nullptr) 
    : SingleChildRenderObjectWidget(child) {}

  RefNauseam<Element> createElement() override;
  RefNauseam<caffeine::RenderObject> createRenderObject() override {
    // TODO: RenderView
    return new RenderProxyBox();
  }

  virtual ~RootWidget() = default;
};

/// The root element of the tree. Has no parent, manages a single child element.
class RootElement : public SingleChildRenderObjectElement {
public:
  RootElement(RootWidget* widget) : SingleChildRenderObjectElement(widget) {}

  Element* getChild() const { return m_child.get(); }

  /// Get the child's render object (if it's a RenderObjectElement)
  RefNauseam<caffeine::RenderObject> getChildRenderObject() const {
    if (m_child) {
      return m_child->getRenderObject();
    }
    return nullptr;
  }

  void assignOwner(const std::shared_ptr<BuildOwner>& owner);
  void visitChildren(ElementVisitor visitor) override;
  void performRebuild() override;
  void update(Widget* newWidget) override;
  void mount(RefNauseam<Element> parent, void* newSlot) override;
};
}
