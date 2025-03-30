#pragma once
#include "Container.hpp"

GDF_NS_START
/// Despite its name, this class only provides convenient setup for a [Container]
/// to behave like a "button", since [Container]s are already clickable (listenable via the MouseEvent).
class ClickableContainer : public Container {
  std::string m_sfx;
  bool m_consumeTap;
public:
  static ClickableContainer *create(std::string sfx, bool consumeTap = false);
  bool init(std::string sfx, bool consumeTap = false);
};

GDF_NS_END