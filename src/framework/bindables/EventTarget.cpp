#include "EventTarget.hpp"

void EventTarget::addListener(std::string eventName, Callback listener) {
    m_listeners[eventName].push_back(listener);
};
void EventTarget::removeListener(std::string eventName, Callback listener) {
    /*
    auto the = m_listeners[eventName];
    // because this fucker causes a C2678 error
    auto it = std::find(the.begin(),the.end(), listener);
    if (it != the.end()) {
        the.erase(it);
    };
    */
}

bool EventTarget::dispatchEvent(NodeEvent* event) {
    if (event == nullptr) return false;
    if (event->m_target == nullptr) event->m_target = this;
    bool ltExist = (m_listeners.find(event->eventName()) != m_listeners.end());
    if (!ltExist) return true;
    event->m_cancelled = false;
    for (auto i : m_listeners[event->eventName()]) {
        i(event);
        
        // @returns true if one of the callback cancels the event, either via `preventDefault()` or `stopImmediatePropagate()` (for `DispatchingFlow::Down`)
        if (
            // stop immediate propagation
            (event->m_stopImmediatePropagate) ||
            // or being a victim on twitter
            event->m_cancelled
        ) return false;
    }
    return true;
}
