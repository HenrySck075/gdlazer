#pragma once
#include "Event.hpp"
#include <functional>
#include <type_traits>
#include <typeindex>

#define typeindex(type) std::type_index(typeid(type))

GDF_NS_START

template<typename T>
concept EventType = std::is_base_of_v<Event, T>;

// just to mask away the templated type
struct EventListenerFunc {
  virtual bool call(Event* event) const = 0;
  virtual bool operator==(const EventListenerFunc&) const = 0;
  virtual void* get() const = 0;
};

namespace impl {
  template<EventType T>
  struct EventListenerFuncTemplated : public EventListenerFunc{
    std::function<bool(T*)> func;
    EventListenerFuncTemplated(std::function<bool(T*)> f)
      : func(f) {}
    bool call(Event* event) const override {
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
      std::make_shared<impl::EventListenerFuncTemplated<T>>(listener)
    );
  };

  template<EventType T>
  void removeListener(const EventListener<T>& listener) {
    auto it = m_listeners.find(typeindex(T));
    if (it != m_listeners.end()) {
      auto &listeners = it->second;
      // Remove all matching listeners
       listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
         [&](std::shared_ptr<EventListenerFunc> l) {
           return l->get() == listener.template target<bool(T*)>();
         }),
      listeners.end());
    }
  };

  template<EventType T>
  bool dispatchEvent(T* event) {
    return doDispatchEvent(event, typeindex(T));
  }
protected:
  virtual bool doDispatchEvent(Event* event, std::type_index type);
  void clearAllListeners();
private:
  std::unordered_map<std::type_index, std::list<std::shared_ptr<EventListenerFunc>>> m_listeners;
};
GDF_NS_END
