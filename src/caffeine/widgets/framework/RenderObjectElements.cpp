#include <gdlazer/caffeine/widgets/framework/RenderObjectElements.hpp>

namespace caffeine {

void RenderObjectElement::mount(std::shared_ptr<Element> parent, void* slot) {
  Element::mount(parent, slot);
  m_ancestorRenderObjectElement = findAncestorRenderObjectElement();
  auto widget = std::static_pointer_cast<RenderObjectWidget>(m_widget);
  m_renderObject = widget->createRenderObject();
  attachRenderObject(slot);
  Element::performRebuild();
}

std::shared_ptr<RenderObjectElement> RenderObjectElement::findAncestorRenderObjectElement() {
  auto current = m_parent;
  while (current) {
    auto renderObjElem = std::dynamic_pointer_cast<RenderObjectElement>(current);
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

std::shared_ptr<caffeine::RenderObject> RenderObjectElement::getRenderObject() {
  return m_renderObject;
};



void SingleChildRenderObjectElement::performRebuild() {
  auto widget = std::static_pointer_cast<SingleChildRenderObjectWidget>(m_widget);
  Widget* childWidget = const_cast<Widget*>(widget->getChild());
  Element::performRebuild();
  try {
    m_child = updateChild(m_child, childWidget, m_slot);
  } catch (std::exception&) {
    if (m_child) m_child->deactivate();
    m_child = updateChild(nullptr, childWidget, m_slot);
  }
}

void SingleChildRenderObjectElement::update(Widget* newWidget) {
  Element::update(newWidget);
  auto widget = std::static_pointer_cast<SingleChildRenderObjectWidget>(m_widget);
  m_child = updateChild(m_child, const_cast<Widget*>(widget->getChild()), nullptr);
}

std::shared_ptr<Element> SingleChildRenderObjectElement::getAttachingRenderObjectChild() {
  return m_child;
}

void SingleChildRenderObjectElement::visitChildren(std::function<void(std::shared_ptr<Element>)> visitor) {
  if (m_child) {
    visitor(m_child);
  }
}
void SingleChildRenderObjectElement::mount(std::shared_ptr<Element> parent, void *slot) {
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
  auto widget = std::static_pointer_cast<MultiChildRenderObjectWidget>(m_widget);
  const auto& widgets = widget->getChildren();
  Element::performRebuild();

  try {
    // TODO: Implement proper child reconciliation algorithm
    // For now, we'll do a simple approach: rebuild children in order
    while (m_children.size() > widgets.size()) {
      auto child = m_children.back();
      m_children.pop_back();
      child->deactivate();
    }

    for (size_t i = 0; i < widgets.size(); ++i) {
      std::shared_ptr<Element> child;
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
  } catch (std::exception&) {
    for (auto& child : m_children) {
      if (child) child->deactivate();
    }
    m_children.clear();
    // Attempt to rebuild with no children
    for (size_t i = 0; i < widgets.size(); ++i) {
      auto child = updateChild(nullptr, const_cast<Widget*>(widgets[i].get()), reinterpret_cast<void*>(i));
      m_children.push_back(child);
    }
  }
}

void MultiChildRenderObjectElement::update(Widget* newWidget) {
  Element::update(newWidget);
  // TODO: Element::updateChildren
}

void MultiChildRenderObjectElement::visitChildren(std::function<void(std::shared_ptr<Element>)> visitor) {
  for (auto& child : m_children) {
    if (child) {
      visitor(child);
    }
  }
}

void MultiChildRenderObjectElement::mount(std::shared_ptr<Element> parent, void *slot) {
  RenderObjectElement::mount(parent, slot);
  auto widget = std::static_pointer_cast<MultiChildRenderObjectWidget>(m_widget);
  const auto& widgets = widget->getChildren();
  for (size_t i = 0; i < widgets.size(); ++i) {
    auto child = inflateWidget(
      const_cast<Widget*>(widgets[i].get()),
      reinterpret_cast<void*>(i)
    );
    m_children.push_back(child);
  }
};

void MultiChildRenderObjectElement::insertRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) {
  // TODO: implement render object tree integration
}

void MultiChildRenderObjectElement::removeRenderObjectChild(std::shared_ptr<caffeine::RenderObject> child) {
  // TODO: implement render object tree integration
}
} // namespace caffeine
