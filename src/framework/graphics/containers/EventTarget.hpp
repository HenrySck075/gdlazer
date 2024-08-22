#pragma once
#include "Event.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ISupportsChildrenGet {
public:
  virtual CCArray* getChildren() = 0;
};

// The base implementation for a class (preferably in a node tree) that receives events
class EventTarget {
public:
    using Callback = geode::utils::MiniFunction<void(NodeEvent*)>;
private:
    std::map<std::string, std::vector<Callback>> m_listeners;
protected:
    // @returns true if one of the callback cancels the event, either via `preventDefault()` or `stopImmediatePropagate()` (for `DispatchingFlow::Down`)
    virtual bool tryDispatch(Callback& cb, NodeEvent* event);
    // dispatchEvent but does not care about whether any listener even registered that event 
    //
    // used by dispatchEvent to handle actually calling listeners
    //
    // trust me its better than reloadAllStep5
    virtual bool dispatchEventUnsafe(NodeEvent* event);

    void updateDispatchFlow(NodeEvent* event, DispatchingFlow flow) {
        event->m_dispatchingFlow = flow;
    }
public:
    void addListener(std::string eventName, const Callback& listener);
    void removeListener(std::string eventName, const Callback& listener);
    /// @returns true if the event is successfully dispatched or false if any of the handler cancels the event in any way
    virtual bool dispatchEvent(NodeEvent* event);

    friend class Container;
};
