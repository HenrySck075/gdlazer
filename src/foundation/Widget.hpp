#pragma once

#include "shared_ptr_2.hpp"
class Element;

class Widget {
  
public:
  virtual std::shared_ptr<Element> createElement();
  static bool canUpdate(shared_ptr_ctor<Widget> newWidget, shared_ptr_ctor<Widget> oldWidget);
};
