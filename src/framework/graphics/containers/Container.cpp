#include "Container.hpp"

void Container::addEventListener(std::string eventName, Callback listener) {
    m_listeners[eventName].push_back(listener);
};
void Container::removeEventListener(std::string eventName, Callback listener) {
    auto the = m_listeners[eventName];
    auto it = std::find(the.begin(),the.end(), listener);
    if (it != the.end()) {
        the.erase(it);
    }
}
void Container::dispatchEvent(NodeEvent* event) {
    if (std::find(m_listeners.begin(),m_listeners.end(),event->eventName())==m_listeners.end()) {
        return;
    }
    for (auto i : m_listeners[event->eventName()]) {
        i(event);
    }
};

void Container::dispatchToChild(NodeEvent* event) {
    CCObject* obj;
    CCARRAY_FOREACH(m_pChildren, obj) {
        if (auto node = dynamic_cast<Node*>(obj)) {
            node->dispatchEvent(event);
        }
    }
}