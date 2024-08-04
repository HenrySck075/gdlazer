// we javascripting
// @note Not on osu-framework. But if you are going to argue, this is Container

#include <Geode/Geode.hpp>
#include "Event.hpp"

using namespace geode::prelude;

/// @brief CCNode(Container) that implements local Event system (like javascript EventTarget)
class Node : public CCNodeContainer {
private:
    using Callback = std::function<void(NodeEvent&)>;
    std::map<std::string, std::vector<Callback>> m_listeners;
public:
    void addEventListener(std::string eventName, Callback listener);
    void removeEventListener(std::string eventName, Callback listener);
    void dispatchEvent(NodeEvent& event);
};