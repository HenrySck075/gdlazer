#include "Element.hpp"
#include "Container.hpp"
#include "assert.h"

void Element::update(ContainerPtr newWidget) {
  m_widget = newWidget;
}

void Element::mount(ElementPtr parent, void* slot) {
  assert(m_lifecycle == _ElementLifecycle::initial);
  m_lifecycle = _ElementLifecycle::active;
  m_parent = parent;
  m_depth = parent.get() ? parent->m_depth+1 : 1;
};

cocos2d::CCNode* Element::render() {
  auto node = cocos2d::CCNode::create();
  
  node->setContentSize({m_widget->size.x, m_widget->size.y});

  return node;
};