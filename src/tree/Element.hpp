#pragma once

#ifdef GDF_DEBUG
#undef NDEBUG
#endif

#include <typeindex>
#include "tree/shared_ptr_2.hpp"
struct Container;
struct Element;
using ContainerPtr = shared_ptr_ctor<Container>;
using ElementPtr = shared_ptr_ctor<Element>;
enum class _ElementLifecycle { initial, active, inactive, defunct };

template<class T>
concept IsContainer = std::derived_from<T, Container>;
struct Element {
  Element() = delete;
  Element(ContainerPtr widget) : m_managedType(typeid(widget.get())), m_widget(widget) {};

  void update(ContainerPtr newWidget);
  void mount(ElementPtr maybeParent, void* slot);

  int getDepth() {return m_depth;}

  cocos2d::CCNode* render();
private:
  ElementPtr m_parent;
  std::type_index m_managedType;
  ContainerPtr m_widget;
  bool m_dirty = false;
  void* m_slot = 0;
  int m_depth = -1;
  _ElementLifecycle m_lifecycle = _ElementLifecycle::initial;
};