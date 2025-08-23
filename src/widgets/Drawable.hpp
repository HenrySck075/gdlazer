#pragma once
#include "tree/shared_ptr_2.hpp"
struct Element;
using ElementPtr = shared_ptr_ctor<Element>;


struct Drawable {
  virtual ElementPtr createElement() = 0; /// :thumbsup:
};