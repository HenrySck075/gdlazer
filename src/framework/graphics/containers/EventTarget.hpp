#pragma once
#include "Event.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// The base implementation for a class (preferably in a node tree) that receives events
class EventTarget {
public:
    using Callback = geode::utils::MiniFunction<void(NodeEvent*)>;
private:
    std::map<std::string, std::vector<Callback>> m_listeners;

    // @returns true if one of the callback cancels the event, either via `cancel()` or `stopImmediatePropagate()` (for `DispatchingFlow::Down`)
    bool tryDispatch(Callback& cb, NodeEvent* event);
public:
    void addListener(std::string eventName, const Callback& listener);
    void removeListener(std::string eventName, const Callback& listener);
    virtual void dispatchEvent(NodeEvent* event);
    

};
