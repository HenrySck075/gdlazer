#include "EventTarget.hpp"


void EventTarget::addListener(std::string eventName, const Callback& listener) {
    m_listeners[eventName].push_back(listener);
};
void EventTarget::removeListener(std::string eventName, const Callback& listener) {
    /*
    auto the = m_listeners[eventName];
    auto it = std::find(the.begin(),the.end(), listener);
    if (it != the.end()) {
        the.erase(it);
    }
    */
}

bool EventTarget::tryDispatch(Callback& cb, NodeEvent* event) {
    cb(event);
    return 
      // stop immediate propagation
      (event->m_stopImmediatePropagate) ||
      // or being a victim on twitter
      event->m_cancelled;
}

void EventTarget::dispatchEvent(NodeEvent* event) {
    auto it = m_listeners.find(event->eventName());
    if (it == m_listeners.end()) {
        log::warn("[EventTarget]: Event {} not in list.",event->eventName());
        return;
    }
    dispatchEventUnsafe(event);
};

void EventTarget::dispatchEventUnsafe(NodeEvent* event) {
    for (auto i : m_listeners[event->eventName()]) {
        if (tryDispatch(i,event)) break;
    }
}
