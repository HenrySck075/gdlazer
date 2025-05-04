#include "Event.hpp"

GDF_NS_START

void Event::stopPropagation() {
  //geode::log::debug("[Event] propagate stopped | {}", this);
  m_propagateStopped = true;
};
void Event::stopImmediatePropagation() {
  stopPropagation();
  m_immediatePropagateStopped = true;
};
GDF_NS_END