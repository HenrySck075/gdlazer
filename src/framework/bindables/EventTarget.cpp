#include "EventTarget.hpp"

void EventTarget::addListener(std::string eventName, const Callback& listener) {
    m_listeners[eventName].push_back(listener);
};
void EventTarget::removeListener(std::string eventName, const Callback& listener) {
    auto the = m_listeners[eventName];
    auto it = std::find(the.begin(),the.end(), listener);
    if (it != the.end()) {
        the.erase(it);
    };
}

bool EventTarget::tryDispatch(Callback& cb, NodeEvent* event) {
    cb(event);
    return 
      // stop immediate propagation
      (event->m_stopImmediatePropagate) ||
      // or being a victim on twitter
      event->m_cancelled;
}

bool EventTarget::listenersExists(NodeEvent* event) {
    if (event->m_target == nullptr) event->m_target = this;
    auto it = m_listeners.find(event->eventName());
    return (it != m_listeners.end());
};

bool EventTarget::dispatchEvent(NodeEvent* event) {
    if (event == nullptr) return false;
    bool ltExist = listenersExists(event);
    if (!ltExist) return true;
    event->m_cancelled = false;
    for (auto i : m_listeners[event->eventName()]) {
        if (tryDispatch(i,event)) {
            return false;
        };
    }
    return true;
}
