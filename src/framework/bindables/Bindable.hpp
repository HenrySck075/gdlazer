#include "EventTarget.hpp"
#include "Event.hpp"

namespace {
  extern char const valueChanged[] = "valueChanged";
};

template<typename T>
class ValueChangedEvent : public NamedNodeEvent<valueChanged> {
public:
  const T& value;
  ValueChangedEvent(const T& val) : NamedNodeEvent(), value(val) {}
};

/// An [EventTarget] holding a value
///
/// also this does not automatically retain cocos objects do it yourself
/// TODO: Remove Reactive
template<typename T>
class Bindable : public EventTarget {
  T value;
public:
  Bindable(const T& val) : value(val) {
    log::debug("[Bindable]: hi chat im alive");
  };

  // lel
  T operator->() {return value;}

  T operator=(T C) {
    if (C != value) {
      value = C;
      dispatchEvent(new ValueChangedEvent(C));
    }
    return value;
  }

  operator T() {return value;}

  void addCallback(Callback callback) {
    addListener(valueChanged, callback);
  }

  void removeCallback(Callback callback) {
    removeListener(valueChanged, callback);
  }
};
