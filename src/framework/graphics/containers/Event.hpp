#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Container;
// Event with event name (yea)
//
// For use in Node
class NodeEvent {
protected:
    std::string m_eventName = "";
public:
    NodeEvent() {
        throw "kill yourself";
    }; // provide a name you moron
    NodeEvent(std::string name) : m_eventName(name) {};
    std::string eventName() {return m_eventName;};
    void eventName(std::string newName) {m_eventName = newName;};

    friend class Container;
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
    GeodeEvent(T event) : m_event(event) {
        NodeEvent("geode_"+typeid(event).name());
    }
};