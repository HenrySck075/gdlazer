#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Container;
class EventTarget;
// Event with event name (yea)
//
// For use in Node

enum class DispatchingFlow {Up, Down};

class NodeEvent {
private:
    bool m_stopPropagate = false;
    bool m_stopImmediatePropagate = false;
    DispatchingFlow m_dispatchingFlow = DispatchingFlow::Up;
protected:
    std::string m_eventName = "";
public:
    NodeEvent() {
        throw "kill yourself";
    }; // provide a name you moron
    NodeEvent(std::string name) : m_eventName(name) {};
    std::string eventName() {return m_eventName;};
    void eventName(std::string newName) {m_eventName = newName;};

    void stopPropagate() {m_stopPropagate = true;}
    void stopImmediatePropagate() {m_stopImmediatePropagate = true;}

    friend class Container;
    friend class EventTarget;
};

// wrapper for geode events
//
// event name is "geode_{classname}"
template<is_event T>
class GeodeEvent : public NodeEvent {
private:
    T m_event;
public:
    GeodeEvent(std::string name) = delete;
    GeodeEvent(T event, std::string name) : NodeEvent(name), m_event(event) {}
};
