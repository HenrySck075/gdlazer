#include "EventTarget.hpp"

GDF_NS_START
[[clang::optnone]]
bool EventTarget::doDispatchEvent(Event* event, std::type_index type) {
  geode::Ref<Event> eventRefHolder(event);
  auto listeners = m_listeners.find(type);
  if (listeners != m_listeners.end()) {
    for (std::shared_ptr<EventListenerFunc>& listener : listeners->second) {
      if (!listener->call(event) || event->m_immediatePropagateStopped)
        break;
    }
    return !event->defaultPrevented() || !event->m_immediatePropagateStopped;
  }
  return true;
};
void EventTarget::clearAllListeners() {
  m_listeners.clear();
};

GDF_NS_END
