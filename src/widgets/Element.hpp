#pragma once

#include "Geode/cocos/base_nodes/CCNode.h"
#include "shared_ptr_2.hpp"
#include <memory>

class RenderElement;

class Widget;
class Element;
using ElementPtr = std::shared_ptr<Element>;

class Element {
private:
  RenderElement* getAncestorRenderElement();
  std::shared_ptr<Widget*> m_widget;

public:
  Element(shared_ptr_ctor<Widget> widget) : m_widget(widget) {};

  virtual void attachRender();
}; 
