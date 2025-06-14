#include "Event.hpp"
#include "../../utils.hpp"

GDF_NS_START

void Event::stopPropagation() {
  m_propagateStopped = true;
};
void Event::stopImmediatePropagation() {
  stopPropagation();
  m_immediatePropagateStopped = true;
};
GDF_NS_END