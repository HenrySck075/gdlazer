#include "EventTarget.hpp"
#include "Event.hpp"

namespace {
    extern char const valueChanged[] = "valueChanged";
};

template<typename T>
class ValueChangedEvent : public NamedNodeEvent<valueChanged> {
    const T& value;
public:
    ValueChangedEvent(const T& val) : NamedNodeEvent(), value(val) {}
};

/// An [EventTarget] holding a value
///
/// also this does not automatically retain cocos objects do it yourself
template<typename T>
class Bindable : public EventTarget {
    T value;
public:
    Bindable(const T& val) : value(val) {};

    // lel
    T operator->() {return value;}

    T& operator=(const T& C)
    {
        if (C != value) {
            value = C;
            dispatchEvent(new ValueChangedEvent(C));
        }
    }

    operator T() {return value;}

    void addCallback(Callback callback) {
        addListener(valueChanged, callback);
    }

    void removeCallback(Callback callback) {
        removeListener(valueChanged, callback);
    }
};
