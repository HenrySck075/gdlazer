#include "EventTarget.hpp"

GDL_NS_START
void EventTarget::addListener(const std::string &type,
                                   EventListener listener) {
  m_listeners[type].push_back(std::move(listener));
}
void EventTarget::removeListener(const std::string &type,
                                      const EventListener &listener) {
  auto it = m_listeners.find(type);
  if (it != m_listeners.end()) {
    auto &listeners = it->second;
    // Remove all matching listeners
    listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
                                   [&](const EventListener &l) {
                                     return l.target_type() ==
                                            listener.target_type();
                                   }),
                    listeners.end());
  }
}
bool EventTarget::dispatchEvent(Event* event) {
  event->retain();
  auto it = m_listeners.find(event->type());
  if (it != m_listeners.end()) {
    for (const auto &listener : it->second) {
      listener(event);
    }
  }
  event->release();
  return !event->defaultPrevented();
}
GDL_NS_END
