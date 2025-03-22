#pragma once
#include "Event.hpp"
#include <functional>
#include <type_traits>
#include <typeindex>

#define typeindex(type) std::type_index(typeid(type))

GDL_NS_START

template<typename T>
concept EventType = std::is_base_of_v<Event, T>;

// just to mask away the templated type
struct EventListenerFunc {
  virtual bool operator()(Event* event) const {
    return true;
  };
  virtual bool operator==(const EventListenerFunc&) const {
    return false;
  };
  virtual void* get() const {
    return 0;
  };
};

namespace impl {
  template<EventType T>
  struct EventListenerFuncTemplated : public EventListenerFunc{
    std::function<bool(T*)> func;
    EventListenerFuncTemplated(std::function<bool(T*)> f)
      : func(f) {}
    bool operator()(Event* event) const override {
      return func(static_cast<T*>(event));
    }
    bool operator==(const EventListenerFunc& otter) const override {
      return get() == otter.get();
    }
    void* get() const override {
      return (void*)(func.template target<bool(T*)>());
    }
  };
};

class EventTarget {
public:
    template<EventType T>
    using EventListener = std::function<bool(T*)>;

    template<EventType T>
    void addListener(const EventListener<T>& listener) {
      m_listeners[typeindex(T)].push_back(
        impl::EventListenerFuncTemplated<T>{listener}
      );
    };

    template<EventType T>
    void removeListener(const EventListener<T>& listener) {
      auto it = m_listeners.find(typeindex(T));
      if (it != m_listeners.end()) {
        auto &listeners = it->second;
        // Remove all matching listeners
         listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
           [&](const EventListenerFunc &l) {
             return l == listener;
           }),
        listeners.end());
      }
    };

    template<EventType T>
    bool dispatchEvent(T* event) {
      return doDispatchEvent(event, typeindex(T));
    }
  protected:
    virtual bool doDispatchEvent(Event* event, std::type_index type) {
      auto listeners = m_listeners.find(type);
      if (listeners != m_listeners.end()) {
        event->retain();
        for (const auto &listener : listeners->second) {
          if (!listener(event) || event->m_immediatePropagateStopped) 
            break;
        }
        event->release();
        return !event->defaultPrevented() && !event->m_immediatePropagateStopped;
      }
      return true;
    };
  private:
    std::unordered_map<std::type_index, std::list<EventListenerFunc>> m_listeners;
};
GDL_NS_END
