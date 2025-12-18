#include <gdlazer/caffeine/widgets/framework/RenderObjectElements.hpp>

namespace caffeine {

void RenderObjectElement::mount(RefNauseam<Element> parent, void* slot) {
  Element::mount(parent, slot);
  m_ancestorRenderObjectElement = findAncestorRenderObjectElement();
  auto widget = dynamic_cast<RenderObjectWidget*>(m_widget.get());
  m_renderObject = widget->createRenderObject();
  attachRenderObject(slot);
  Element::performRebuild();
}

RefNauseam<RenderObjectElement> RenderObjectElement::findAncestorRenderObjectElement() {
  auto current = m_parent;
  while (current) {
    auto renderObjElem = dynamic_cast<RenderObjectElement*>(current.get());
    if (renderObjElem && renderObjElem->getRenderObject() != m_renderObject) {
      return renderObjElem;
    }
    current = current->m_parent;
  }
  return nullptr;
}


void RenderObjectElement::attachRenderObject(void* slot) {
  if ((m_ancestorRenderObjectElement = findAncestorRenderObjectElement())) {
    m_ancestorRenderObjectElement->insertRenderObjectChild(m_renderObject);
  }
}

void RenderObjectElement::detachRenderObject() {
  if (m_ancestorRenderObjectElement) {
    m_ancestorRenderObjectElement->removeRenderObjectChild(m_renderObject);
  }
}

RefNauseam<caffeine::RenderObject> RenderObjectElement::getRenderObject() {
  return m_renderObject;
};



void SingleChildRenderObjectElement::performRebuild() {
  auto widget = dynamic_cast<SingleChildRenderObjectWidget*>(m_widget.get());
  Widget* childWidget = const_cast<Widget*>(widget->getChild());
  Element::performRebuild();
  m_child = updateChild(m_child, childWidget, m_slot);
}

void SingleChildRenderObjectElement::update(Widget* newWidget) {
  Element::update(newWidget);
  auto widget = dynamic_cast<SingleChildRenderObjectWidget*>(m_widget.get());
  m_child = updateChild(m_child, const_cast<Widget*>(widget->getChild()), nullptr);
}

RefNauseam<Element> SingleChildRenderObjectElement::getAttachingRenderObjectChild() {
  return m_child;
}

void SingleChildRenderObjectElement::visitChildren(std::function<void(RefNauseam<Element>)> visitor) {
  if (m_child) {
    visitor(m_child);
  }
}
void SingleChildRenderObjectElement::mount(RefNauseam<Element> parent, void *slot) {
  RenderObjectElement::mount(parent, slot);
  m_child = updateChild(
    m_child,
    const_cast<Widget*>(
      static_cast<SingleChildRenderObjectWidget*>(m_widget.get())->getChild()
    ),
    nullptr
  );
};


void MultiChildRenderObjectElement::performRebuild() {
  auto widget = dynamic_cast<MultiChildRenderObjectWidget*>(m_widget.get());
  const auto& widgets = widget->getChildren();
  Element::performRebuild();

  // TODO: Implement proper child reconciliation algorithm
  // For now, we'll do a simple approach: rebuild children in order
  while (m_children.size() > widgets.size()) {
    auto child = m_children.back();
    m_children.pop_back();
    child->deactivate();
  }

  for (size_t i = 0; i < widgets.size(); ++i) {
    RefNauseam<Element> child;
    if (i < m_children.size()) {
      child = m_children[i];
    }
    child = updateChild(child, const_cast<Widget*>(widgets[i].get()), reinterpret_cast<void*>(i));
    if (i < m_children.size()) {
      m_children[i] = child;
    } else {
      m_children.push_back(child);
    }
  }
}

void MultiChildRenderObjectElement::update(Widget* newWidget) {
  Element::update(newWidget);
  // TODO: Element::updateChildren
}

void MultiChildRenderObjectElement::visitChildren(std::function<void(RefNauseam<Element>)> visitor) {
  for (auto& child : m_children) {
    if (child) {
      visitor(child);
    }
  }
}

void MultiChildRenderObjectElement::mount(RefNauseam<Element> parent, void *slot) {
  RenderObjectElement::mount(parent, slot);
  auto widget = dynamic_cast<MultiChildRenderObjectWidget*>(m_widget.get());
  const auto& widgets = widget->getChildren();
  for (size_t i = 0; i < widgets.size(); ++i) {
    auto child = inflateWidget(
      const_cast<Widget*>(widgets[i].get()),
      reinterpret_cast<void*>(i)
    );
    m_children.push_back(child);
  }
};

void MultiChildRenderObjectElement::insertRenderObjectChild(RefNauseam<caffeine::RenderObject> child) {
  // TODO: implement render object tree integration
}

void MultiChildRenderObjectElement::removeRenderObjectChild(RefNauseam<caffeine::RenderObject> child) {
  // TODO: implement render object tree integration
}
} // namespace caffeine
