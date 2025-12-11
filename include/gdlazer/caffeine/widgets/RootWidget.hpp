#pragma once

#include "gdlazer/caffeine/foundation/Element.hpp"
#include "gdlazer/caffeine/foundation/Widget.hpp"

class RootElement;
class BuildOwner;

/// A widget for the root of the widget tree.
/// Wraps the application's root widget and bootstraps the element tree.
class RootWidget : public Widget {
public:
  // Accept raw Widget* and convert to shared_ptr internally
  RootWidget(Widget* child = nullptr) 
    : m_child(std::shared_ptr<Widget>(child)) {}

  const Widget* getChild() const { return m_child.get(); }
  std::shared_ptr<Element> createElement() override;

  virtual ~RootWidget() = default;

private:
  std::shared_ptr<Widget> m_child;
};

/// The root element of the tree. Has no parent, manages a single child element.
class RootElement : public Element {
public:
  RootElement(const std::shared_ptr<RootWidget>& widget) : Element(widget.get()) {}

  Element* getChild() const { return m_child.get(); }

  void assignOwner(const std::shared_ptr<BuildOwner>& owner);
  void visitChildren(ElementVisitor visitor) override;
  void performRebuild() override;
  void update(Widget* newWidget) override;
  void mount(Element* parent, void* newSlot);

protected:
  std::shared_ptr<caffeine::RenderObject> getRenderObject() override { return nullptr; }

private:
  std::shared_ptr<Element> m_child;

  void rebuild();
};
