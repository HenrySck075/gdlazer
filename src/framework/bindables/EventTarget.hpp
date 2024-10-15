#pragma once
#include "Event.hpp"
#include "Geode/utils/MiniFunction.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ISupportsChildrenGet {
public:
  virtual CCArray* getChildren() = 0;
};


// The base implementation for a class (preferably in a node tree) that receives events
// i realized i never made this to be used outside of a node tree
class EventTarget {
public:
    using Callback = std::function<void(NodeEvent*)>;
private:
    std::map<std::string, std::vector<Callback>> m_listeners;
protected:
    // @returns true if one of the callback cancels the event, either via `preventDefault()` or `stopImmediatePropagate()` (for `DispatchingFlow::Down`)
    virtual bool tryDispatch(Callback& cb, NodeEvent* event);
    virtual bool listenersExists(NodeEvent* event);

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
