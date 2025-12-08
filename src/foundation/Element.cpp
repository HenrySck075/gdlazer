#include "gdlazer/caffeine/foundation/utils/shared_ptr_2.hpp"
#include <functional>
#include <gdlazer/caffeine/foundation/BuildOwner.hpp>
#include <gdlazer/caffeine/foundation/utils/massert.h>
#include <gdlazer/caffeine/foundation/Element.hpp>

cocos2d::CCNode *Element::getRenderObject() {
  auto child = getAttachingRenderObjectChild();
  if (child) {
    return child->getRenderObject();
  }
  return nullptr;
};
std::shared_ptr<Element> Element::getAttachingRenderObjectChild() {
  std::shared_ptr<Element> next;
  visitChildren([&next](shared_ptr_ctor<Element> node) {
    assert(!next);
    next = node;
  });
  return next;
}
void Element::attachRenderObject() {
  auto crack = [](shared_ptr_ctor<Element> child) {
    child->attachRenderObject();
  };
  visitChildren(crack);
}
void Element::detachRenderObject() {
  visitChildren([](shared_ptr_ctor<Element> child) {
    child->detachRenderObject();
  });
}

void Element::mount(shared_ptr_ctor<Element> parent, void* slot) {
  massert(m_lifecycleState == _ElementLifecycle::initial, "This element is no longer in its initial state.");
  massert(
    m_parent == nullptr,
    "This element already has a parent and it shouldn't have one yet."
  );
  massert(
    parent == nullptr || parent->m_lifecycleState == _ElementLifecycle::active,
    "Parent should be null or in the active state"
  );
  m_parent = parent;
  m_lifecycleState = _ElementLifecycle::active;
  m_slot = slot;
  if (auto key = dynamic_cast<GlobalKeyU*>(m_widget->m_key->get())) {
    m_owner->_registerGlobalKey(key, this);
  }
}
void Element::unmount() {
  assert(m_lifecycleState == _ElementLifecycle::active);
  assert(m_widget);
  assert(m_owner);
  if (auto k = dynamic_cast<GlobalKeyU*>(m_widget->m_key->get())) {
    m_owner->_unregisterGlobalKey(k, this);
  }
}

std::shared_ptr<BuildScope>& Element::getBuildScope() {
  return m_parentBuildScope;
};

void Element::markNeedsBuild() {
  m_owner->scheduleBuildFor(this);
}

void Element::performRebuild() { m_dirty = false; };
void Element::rebuild() {
  assert(m_lifecycleState != _ElementLifecycle::initial);
  performRebuild();
}

std::shared_ptr<Element> Element::_retakeInactiveElement(
  GlobalKeyU* key, Widget* widget
) {
  std::shared_ptr<Element> ce(static_cast<Element*>(key->m_currentContext.get()));
  if (!ce) return nullptr;
  if (!Widget::canUpdate(widget, ce->m_widget.get())) return nullptr;
  auto parent = ce->m_parent;
  if (parent) {
    parent->deactivateChild(ce.get());
  }
  assert(ce->m_parent == nullptr);
  m_owner->m_inactiveElements.remove(ce);
  return ce;
}

std::shared_ptr<Element> Element::inflateWidget(
  std::shared_ptr<Widget> newWidget,
  void* newSlot
) {
  const auto key = newWidget->m_key;
  auto maybeGlobalKey = dynamic_cast<GlobalKeyU*>(key->get());
  auto inactiveElement = maybeGlobalKey ? _retakeInactiveElement(maybeGlobalKey, newWidget.get()) : nullptr;
  if (inactiveElement) {
    assert(inactiveElement->m_parent == nullptr);
    inactiveElement->_activateWithParent(this, newSlot);
    auto up = updateChild(inactiveElement, newWidget, newSlot);
    assert(up == inactiveElement);
    return up;
  }
  else {
    auto newElement = newWidget->createElement();
    newElement->mount(this, newSlot);
    assert(newElement->m_lifecycleState == _ElementLifecycle::active);
    return newElement;
  }
}


std::shared_ptr<Element> Element::updateChild(
  shared_ptr_ctor<Element> child,
  shared_ptr_ctor<Widget> newWidget,
  void* newSlot
) {
  if (child == nullptr) {
    if (newWidget != nullptr) {
      auto newChild = inflateWidget(newWidget, newSlot);
      return newChild;
    } else {
      return nullptr;
    }
  } else if (newWidget == nullptr) {
    deactivateChild(child);
    return nullptr;
  } else if (Widget::canUpdate(newWidget, child->m_widget)) {
    if (child->m_slot != newSlot) updateSlotForChild(child, newSlot);
    child->update(newWidget);
    assert(child->m_widget == newWidget);
    return child;
  } else {
    deactivateChild(child);
    auto newChild = inflateWidget(newWidget, newSlot);
    return newChild;
  }

}
void Element::updateSlot(void* slot) {
  assert(m_lifecycleState == _ElementLifecycle::active);
  assert(m_parent);
  assert(m_parent->m_lifecycleState == _ElementLifecycle::active);
  m_slot = slot;
}
void Element::updateSlotForChild(
  shared_ptr_ctor<Element> child,
  void* slot
) {
  assert(m_lifecycleState == _ElementLifecycle::active);
  assert(child->m_parent.get() == this);
  const std::function<void(std::shared_ptr<Element>)> visit = [&slot, &visit](std::shared_ptr<Element> e){
    e->updateSlot(slot);
    if (auto descendant = e->getAttachingRenderObjectChild()) {
      visit(descendant);
    }
  };
  visit(child);
};

void Element::deactivateChild(shared_ptr_ctor<Element> child) {
  child->detachRenderObject();
}

void Element::activate() {
  assert(m_lifecycleState == _ElementLifecycle::inactive);
  m_lifecycleState = _ElementLifecycle::active;
  if (m_dirty) {
    m_owner->scheduleBuildFor(this);
  }
};


std::shared_ptr<RenderObjectElement> RenderObjectElement::findAncestorRenderObjectElement() {
  // traverse up the parents to find a renderObject that is not this one's
  // renderObject
  auto current = m_parent;
  while (current) {
    auto c = dynamic_cast<RenderObjectElement*>(current.get());
    if (c && c->getRenderObject() != m_renderObject) {
      return std::shared_ptr<RenderObjectElement>(c);
    }
    current = current->m_parent;
  }
  return nullptr;
}

void RenderObjectElement::attachRenderObject() {
  if (!m_renderObject)
    m_renderObject = createRenderObject();
  if ((m_ancestorRenderObjectElement = findAncestorRenderObjectElement())) {
    m_ancestorRenderObjectElement->insertRenderObjectChild(m_renderObject);
    postAttachRenderObject();
  }
}

void RenderObjectElement::detachRenderObject() {
  if (m_ancestorRenderObjectElement) {
    m_ancestorRenderObjectElement->removeRenderObjectChild(m_renderObject);
  }
}


void _InactiveElements::_deactivateRecursively(shared_ptr_ctor<Element> element) {
  assert(element->m_lifecycleState == _ElementLifecycle::active);
  element->deactivate();
  element->visitChildren(
    std::bind(
      &_InactiveElements::_deactivateRecursively,
      this, std::placeholders::_1
    )
  );
};

void _InactiveElements::add(std::shared_ptr<Element> element) {
  assert(m_locked);
  assert(std::find(m_elements.begin(),m_elements.end(),element)!=m_elements.end());
  assert(element->m_parent == nullptr);

  switch (element->m_lifecycleState) {
    case _ElementLifecycle::active:
      _deactivateRecursively(element.get());
    case _ElementLifecycle::inactive:
      m_elements.push_back(element);
      break;
    default:
      massert(false, "Element must not be deactivated.");
      break;
  }
};

void _InactiveElements::remove(std::shared_ptr<Element> element) {
  assert(!m_locked);
  assert(std::find(m_elements.begin(),m_elements.end(),element)!=m_elements.end());
  assert(element->m_parent == nullptr);
  m_elements.remove(element);
  assert(element->m_lifecycleState == _ElementLifecycle::inactive);
}

void _InactiveElements::_unmount(std::shared_ptr<Element> element) {
  assert(element->m_lifecycleState == _ElementLifecycle::inactive);
  element->visitChildren([](shared_ptr_ctor<Element> e){
    assert(e->m_parent == e);
    _unmount(e);
  });
  element->unmount();
};
void _InactiveElements::_unmountAll() {
  m_locked = true;
  m_elements.sort(Element::_sort);
  m_elements.reverse();
  for (auto& element : m_elements) {
    _unmount(element);
  }
  m_elements.clear();
  m_locked = false;
};
