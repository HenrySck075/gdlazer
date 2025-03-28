#pragma once
#include "Event.hpp"
#include <functional>
#include <list>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <variant>

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
  virtual bool call(Event* event) = 0;
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
  public:
    using VoidCallback = std::function<void(T*)>;
    using BoolCallback = std::function<bool(T*)>;
    using Callback = std::variant<
      VoidCallback, BoolCallback
    >;
  private:
    Callback func;
  public:
    
    EventListenerFuncTemplated(std::function<void(T*)> f)
    : func(f) {}
    EventListenerFuncTemplated(std::function<bool(T*)> f)
    : func(f) {}
    virtual bool call(Event* event) override {
      auto seven = static_cast<T*>(event);
      if (std::holds_alternative<VoidCallback>(func)) {
        std::get<VoidCallback>(func)(seven);
        return true;
      } else {
        return std::get<BoolCallback>(func)(seven);
      }
    }
    virtual bool operator==(const EventListenerFunc& otter) const override {
      return get() == otter.get();
    }
    virtual void* get() const override {
      if (auto voidfunc = std::get_if<VoidCallback>(&func)) return (void*)(voidfunc->template target<void(T*)>());
      if (auto boolfunc = std::get_if<BoolCallback>(&func)) return (void*)(boolfunc->template target<bool(T*)>());
    }
  };
};

class EventTarget {
  friend class Game;
  bool m_dispatching = false;
public:
    template<EventType T>
    using VoidEventListener = impl::EventListenerFuncTemplated<T>::VoidCallback;
    template<EventType T>
    using BoolEventListener = impl::EventListenerFuncTemplated<T>::BoolCallback;

    template<EventType T>
    /// Adds a listener for an event.
    ///
    /// The function can also optionally returns a bool indicating whether or not the handler should continue processing event, without affecting the propagation state of the event itself.
    /// Otherwise, it is the same as returning true.
    void addListener(const VoidEventListener<T>& listener) {
      m_listeners[typeindex(T)].push_back(
        std::make_shared<impl::EventListenerFuncTemplated<T>>(listener)
      );
    };
    template<EventType T>
    /// Adds a listener for an event.
    ///
    /// The function can also optionally returns a bool indicating whether or not the handler should continue processing event, without affecting the propagation state of the event itself.
    /// Otherwise, it is the same as returning true.
    void addListener(const BoolEventListener<T>& listener) {
      m_listeners[typeindex(T)].push_back(
        std::make_shared<impl::EventListenerFuncTemplated<T>>(listener)
      );
    };


      /*
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
      */

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
