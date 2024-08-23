#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Container;
class EventTarget;

// nonsensical name but i hope you guys get it :pleading_face:
enum class DispatchingFlow {Up, Down};

// Event with event name (yea)
//
// For use in EventTarget
class NodeEvent {
private:
    bool m_stopPropagate = false;
    bool m_stopImmediatePropagate = false;
    bool m_cancelled = false;
    DispatchingFlow m_dispatchingFlow = DispatchingFlow::Up;
    EventTarget* m_target = nullptr;

protected:
    bool m_log = false;
    std::string m_eventName = "";
public:
    EventTarget* getCaller() {return m_target;};
    NodeEvent() {
        throw "kill yourself";
    }; // i cant delete this
    NodeEvent(std::string name) : m_eventName(name) {};
    virtual std::string eventName() {return m_eventName;};
    virtual void eventName(std::string newName) {m_eventName = newName;};

    void setDispatchingFlow(DispatchingFlow flow) {m_dispatchingFlow = flow;}

    // Stop any further event dispatchs further in the tree
    void stopPropagation() {m_stopPropagate = true;}
    // Stop any further event dispatchs further in the tree AND the current child list
    void stopImmediatePropagation() {m_stopImmediatePropagate = true;}
    // Cancels the event
    void preventDefault() {m_cancelled = true;}

    friend class Container;
    friend class EventTarget;
};

// wrapper for geode events
//
// event name is "geode_{classname}" (almost)
template<is_event T>
class GeodeEvent : public NodeEvent {
private:
    T m_event;
public:
    GeodeEvent(std::string name) = delete;
    GeodeEvent(T event, std::string name) : NodeEvent(name), m_event(event) {}
};

template<char const* eventname>
class NamedNodeEvent : public NodeEvent {
public:
    NamedNodeEvent() : NodeEvent(eventname) {}
    static std::string eventName() override {return std::string(eventname);};
};