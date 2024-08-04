#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Container;
// Event with event name (yea)
//
// For use in Node
class NodeEvent : public Event {
protected:
    std::string m_eventName = "";
public:
    NodeEvent() : m_eventName(typeid(this).name()) {};
    NodeEvent(std::string name) : m_eventName(name) {};
    std::string eventName() {return m_eventName;};
    void eventName(std::string newName) {m_eventName = newName;};

    friend class Container;
};