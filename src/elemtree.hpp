#pragma once

#include "tree/Container.hpp"

class ElementTree {
  std::shared_ptr<Element> m_root;
public:
  ElementTree() = delete;
  ElementTree(ContainerPtr root) : m_root(root->createElement()) {}
};