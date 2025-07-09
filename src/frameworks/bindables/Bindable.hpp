#pragma once

#include "../../macro.h"
#include <list>

GDF_NS_START

template<typename T>
concept LegallyBindable = 
  requires(T one, T two) {
    one == two; // ????????
  };

template<LegallyBindable T>
/// A struct that allows you to listen to its state's change.
///
/// When the value changes, you can access the object's old value before all the callbacks are processed.
struct Bindable {
private:
  T m_value;
  using Callback = std::function<void(T oldV, T newV)>;
  std::list<Callback> m_listeners;
public:
  Bindable() : m_value(T()) {}
  Bindable(T value) : m_value(value) {};

  operator T () {return m_value;}

  Bindable<T>& operator=(const T& other) {
    if (m_value != other) {
      T oldValue = m_value;
      m_value = other;
      for (auto& l : m_listeners) {
        l(oldValue, other);
      }
    }
    return *this;
  }
  Bindable<T>& operator=(const Bindable<T>& other) {
    return this->operator=(other.m_value);
  }
  void silentSet(const T& other) {
    m_value = other;
  }
  void addCallback(Callback listener) {
    m_listeners.push_back(listener);
  }
  T& operator->() {return m_value;}
  const T& operator->() const {return m_value;}
  T& get() {return m_value;}
  void removeCallback(Callback listener) {
    m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(), [listener](auto& l) {
      return l == listener;
    }), m_listeners.end());
  }
};

GDF_NS_END
