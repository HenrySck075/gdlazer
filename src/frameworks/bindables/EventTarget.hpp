#pragma once
#include "Event.hpp"
#include <string>

GDL_NS_START

class EventTarget {
public:
    using EventListener = std::function<bool(Event*)>;
    void addListener(const std::string &type, EventListener listener);

    void removeListener(const std::string &type,
                             const EventListener& listener);

    virtual bool dispatchEvent(Event* event);

  private:
    std::unordered_map<std::string, std::list<EventListener>> m_listeners;
};
GDL_NS_END