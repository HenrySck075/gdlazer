#pragma once
#include "Event.hpp"
#include <functional>
#include <list>
#include <memory>
#include <type_traits>
#include <typeindex>

#define typeindex(type) std::type_index(typeid(type))

template<>
class fmt::formatter<std::type_index> {
public:
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
  template <typename Context>
  constexpr auto format(std::type_index const& type, Context& ctx) const {
    return fmt::format_to(ctx.out(), "<type {}>", type.name());
  }
};

GDF_NS_START

template<typename T>
concept EventType = std::is_base_of_v<Event, T>;

// just to mask away the templated type
class EventListenerFunc {
public:
  virtual void call(Event* event) = 0;
  virtual bool operator==(const EventListenerFunc&) const {
    return false;
  };
  virtual void* get() const {
    return 0;
  };
};

namespace impl {
  template<EventType T>
  class EventListenerFuncTemplated : public EventListenerFunc {
    std::function<void(T*)> func;
  public:
    EventListenerFuncTemplated(std::function<void(T*)> f)
    : func(f) {
    }
    // im still wondering why wont this work
    virtual void call(Event* event) override {
      func(static_cast<T*>(event));
    }
    virtual bool operator==(const EventListenerFunc& otter) const override {
      return get() == otter.get();
    }
    virtual void* get() const override {
      return (void*)(func.template target<void(T*)>());
    }
  };
};

class EventTarget {
  friend class Game;
  bool m_dispatching = false;
public:
    template<EventType T>
    using EventListener = std::function<void(T*)>;

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
           [&](const EventListenerFunc &l) {
             return l == listener;
           }),
        listeners.end());
      }
    };

    template<EventType T>
    inline bool dispatchEvent(T* event) {
      return doDispatchEvent(event, typeindex(T));
    }
  protected:
    virtual bool doDispatchEvent(Event *event, std::type_index type);

  private:
    std::unordered_map<std::type_index, std::list<std::shared_ptr<EventListenerFunc>>> m_listeners;
};
GDF_NS_END
