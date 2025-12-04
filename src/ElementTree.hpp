#pragma once

#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>
#include "foundation/Element.hpp"
#include "foundation/Widget.hpp"

class ElementTree : public cocos2d::CCScene {
  std::shared_ptr<Element> m_root;
  _InactiveElements m_inactiveElements;
  friend class Element;
public:
  ElementTree() = delete;
  ElementTree(shared_ptr_ctor<Widget> root) : m_root(root->createElement()) {}

  void scheduleBuildFor(shared_ptr_ctor<Element> element);
};
