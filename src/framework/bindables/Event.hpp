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
class NodeEvent : public CCObject {
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
  EventTarget* target() {return m_target;};
  /// please dont
  NodeEvent() {
    throw std::runtime_error("Event must have a name (cant make this deleted or intellisense will scream at me)");
  }; 
  NodeEvent(std::string name) : m_eventName(name) {
    autorelease();
  };
  virtual std::string eventName() {return m_eventName;};
  virtual void eventName(std::string newName) {m_eventName = newName;};

  void setDispatchingFlow(DispatchingFlow flow) {m_dispatchingFlow = flow;}

  // Stop any further event dispatchs further in the tree
  void stopPropagation() {m_stopPropagate = true;}
  // Stop any further event dispatchs further in the tree AND the current child list
  void stopImmediatePropagation() {m_stopImmediatePropagate = true;}
  // Cancels the event
  void preventDefault() {m_cancelled = true;}

  void logging(bool enable) {m_log = enable;}

  friend class Container;
  friend class EventTarget;
  friend class OsuGame;
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

/// TODO: Use Event typeinfo for event name
template<char const* name>
class NamedNodeEvent : public NodeEvent {
public:
  NamedNodeEvent() : NodeEvent(name) {}
  ///! Use this to get the name of the event
  static constexpr auto eventname = name;
};
