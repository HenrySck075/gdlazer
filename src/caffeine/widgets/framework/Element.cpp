#include "gdlazer/caffeine/foundation/log.hpp"
#include <gdlazer/caffeine/foundation/utils/shared_ptr_2.hpp>
#include <functional>
#include <gdlazer/caffeine/widgets/binding/BuildOwner.hpp>
#include <gdlazer/caffeine/foundation/utils/massert.h>
#include <gdlazer/caffeine/widgets/framework/Element.hpp>
#include <gdlazer/caffeine/widgets/framework/Widget.hpp>

namespace caffeine {
Element::Element(Widget* widget) : m_widget(std::shared_ptr<Widget>(widget)) {}

std::shared_ptr<caffeine::RenderObject> Element::getRenderObject() {
  auto child = getAttachingRenderObjectChild();
  if (child) {
    return child->getRenderObject();
  }
  return nullptr;
};
std::shared_ptr<Element> Element::getAttachingRenderObjectChild() {
  std::shared_ptr<Element> next;
  visitChildren([&next](std::shared_ptr<Element> node) {
    assert(!next);
    next = node;
  });
  return next;
}
void Element::attachRenderObject(void* slot) {
  auto crack = [slot](std::shared_ptr<Element> child) {
    child->attachRenderObject(slot);
  };
  visitChildren(crack);
}
void Element::detachRenderObject() {
  visitChildren([](std::shared_ptr<Element> child) {
    child->detachRenderObject();
  });
}

void Element::mount(std::shared_ptr<Element> parent, void* slot) {
  massert(m_lifecycleState == _ElementLifecycle::initial, "This element is no longer in its initial state.");
  massert(
    m_parent == nullptr,
    "This element already has a parent and it shouldn't have one yet."
  );
  geode::log::debug("[Element::mount]: {}", parent == nullptr);
  massert(
    parent == nullptr || parent->m_lifecycleState == _ElementLifecycle::active,
    "Parent should be null or in the active state"
  );
  m_parent = parent;
  m_lifecycleState = _ElementLifecycle::active;
  m_slot = slot;
  
  // assign buildscope: if root element, use owner's root scope, else use parent's scope
  if (parent) {
    m_owner = parent->m_owner;
    m_parentBuildScope = parent->m_parentBuildScope;
  } 
  
  if (auto key = dynamic_cast<GlobalKeyU*>(m_widget->getKey().get())) {
    m_owner->_registerGlobalKey(key, this);
  }
}
void Element::unmount() {
  assert(m_lifecycleState == _ElementLifecycle::active);
  assert(m_widget);
  assert(m_owner);
  if (auto k = dynamic_cast<GlobalKeyU*>(m_widget->getKey().get())) {
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
    parent->deactivateChild(ce);
  }
  assert(ce->m_parent == nullptr);
  m_owner->m_inactiveElements.remove(ce);
  return ce;
}

std::shared_ptr<Element> Element::inflateWidget(
  Widget* newWidget,
  void* newSlot
) {
  const auto key = newWidget->getKey();
  auto maybeGlobalKey = dynamic_cast<GlobalKeyU*>(key.get());
  auto inactiveElement = maybeGlobalKey ? _retakeInactiveElement(maybeGlobalKey, newWidget) : nullptr;
  if (inactiveElement) {
    assert(inactiveElement->m_parent == nullptr);
    inactiveElement->_activateWithParent(this, newSlot);
    auto up = updateChild(inactiveElement, newWidget, newSlot);
    assert(up == inactiveElement);
    return up;
  }
  else {
    auto newElement = newWidget->createElement();
    newElement->mount(shared_from_this(), newSlot);
    assert(newElement->m_lifecycleState == _ElementLifecycle::active);
    return newElement;
  }
}


std::shared_ptr<Element> Element::updateChild(
  std::shared_ptr<Element> child,
  Widget* newWidget,
  void* newSlot
) {
  auto orb = log::getObjectName(this);
  if (child == nullptr) {
    if (newWidget != nullptr) {
      geode::log::debug("[{}::updateChild]: Inflating new element", orb);
      auto newChild = inflateWidget(newWidget, newSlot);
      return newChild;
    } else {
      geode::log::debug("[{}::updateChild]: nothing happens", orb);
      return nullptr;
    }
  } else if (newWidget == nullptr) {
    geode::log::debug("[{}::updateChild]: Deleting {}", orb, child->toString());
    deactivateChild(child);
    return nullptr;
  } else if (Widget::canUpdate(newWidget, child->m_widget.get())) {
    geode::log::debug("[{}::updateChild]: Updating {}", orb, child->toString());
    if (child->m_slot != newSlot) updateSlotForChild(child, newSlot);
    child->update(newWidget);
    assert(child->m_widget.get() == newWidget);
    return child;
  } else {
    geode::log::debug("[{}::updateChild]: Child not updatable, inflating new one", orb);
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
  std::shared_ptr<Element> child,
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

void Element::deactivateChild(std::shared_ptr<Element> child) {
  child->detachRenderObject();
}

void Element::activate() {
  assert(m_lifecycleState == _ElementLifecycle::inactive);
  m_lifecycleState = _ElementLifecycle::active;
  if (m_dirty) {
    m_owner->scheduleBuildFor(this);
  }
};

void Element::_activateWithParent(Element* parent, void* slot) {
  m_parent = parent->shared_from_this();
  m_slot = slot;
  m_lifecycleState = _ElementLifecycle::active;
  _activateRecusively();
}

void Element::_activateRecusively() {
  // Mark this element as active and recursively activate children
  m_lifecycleState = _ElementLifecycle::active;
  visitChildren([](std::shared_ptr<Element> child) {
    child->_activateRecusively();
  });
}
void Element::update(Widget *newWidget) { 
  assert(m_lifecycleState == _ElementLifecycle::active && newWidget != nullptr && Widget::canUpdate(newWidget, m_widget.get()));
  m_widget = newWidget->shared_from_this(); 
};

void _InactiveElements::_deactivateRecursively(std::shared_ptr<Element> element) {
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
      _deactivateRecursively(element);
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
  element->visitChildren([](std::shared_ptr<Element> e){
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


}
