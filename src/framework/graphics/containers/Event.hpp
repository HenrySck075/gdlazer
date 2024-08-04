#include <Geode/Geode.hpp>
using namespace geode::prelude;

// Event with event name (yea)
//
// For use in Node
class NodeEvent : public Event {
private:
    std::string m_eventName = "";
public:
    NodeEvent(std::string name) {
        m_eventName = name;
    }
    std::string eventName() {return m_eventName;};
    void eventName(std::string newName) {m_eventName = newName;};

    friend class Node;
};