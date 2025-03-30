#include "EventTarget.hpp"

GDF_NS_START
bool EventTarget::doDispatchEvent(Event *event, std::type_index type) {
  auto listeners = m_listeners.find(type);
  if (listeners != m_listeners.end()) {
    event->retain();
    for (auto &listener : listeners->second) {
      if (!listener->call(event) || event->m_immediatePropagateStopped)
        break;
    }
    event->release();
    return !event->defaultPrevented() || !event->m_immediatePropagateStopped;
  }
  return true;
};
GDF_NS_END
