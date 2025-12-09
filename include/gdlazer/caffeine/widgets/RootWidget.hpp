#pragma once

#include "gdlazer/caffeine/foundation/Element.hpp"
#include "gdlazer/caffeine/foundation/Widget.hpp"

class RootElement;

/// A widget for the root of the widget tree.
/// Wraps the application's root widget and bootstraps the element tree.
class RootWidget : public Widget, public std::enable_shared_from_this<RootWidget> {
public:
  RootWidget(Widget* child = nullptr) : m_child(child) {}

  Widget* getChild() const { return m_child; }
  std::shared_ptr<Element> createElement() override;

private:
  Widget* m_child;
};

/// The root element of the tree. Has no parent, manages a single child element.
class RootElement : public Element {
public:
  RootElement(const std::shared_ptr<RootWidget>& widget) : Element(widget.get()) {}

  Element* getChild() const { return m_child.get(); }

  void visitChildren(ElementVisitor visitor) override;
  void performRebuild() override;
  void update(Widget* newWidget) override;
  void mount(Element* parent, void* newSlot);

protected:
  cocos2d::CCNode* getRenderObject() override { return nullptr; }

private:
  std::shared_ptr<Element> m_child;

  void rebuild();
};
