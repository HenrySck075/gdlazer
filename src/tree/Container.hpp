#pragma once
#include "RectangleF.hpp"
#include "col/Color4.hpp"
#include "shared_ptr_2.hpp"
#include "types/Vector2.hpp"
#include <typeindex>

struct Container;
struct Element;
using ContainerPtr = shared_ptr_ctor<Container>;
using ElementPtr = shared_ptr_ctor<Element>;

template<class T>
concept IsContainer = std::derived_from<T, Container>;
struct Element {
  Element() = delete;
  template<IsContainer C>
  Element(std::shared_ptr<C> widget) : m_managedType(typeid(C)) {};
private:
  ElementPtr m_parent;
  std::type_index m_managedType;
  bool m_dirty = false;
};

struct Container {
  Vector2 size;
  RectangleF padding;
  Color4 backgroundColor;
  std::vector<ContainerPtr> children;
  std::string id;
  std::string classes;

  virtual Element* createElement();
};