#pragma once
#include "RectangleF.hpp"
#include "col/Color4.hpp"
#include "shared_ptr_2.hpp"
#include "types/Vector2.hpp"
#include "Key.hpp"
#include "Drawable.hpp"

struct Container;
using ContainerPtr = shared_ptr_ctor<Container>;

struct Container : public Drawable {
  std::optional<Key> key = std::nullopt;
  Vector2 size;
  RectangleF padding;
  Vector4 constraints;
  Color4 backgroundColor;
  std::vector<ContainerPtr> children;
  std::string id;
  std::string classes;

  ElementPtr createElement() override;
};