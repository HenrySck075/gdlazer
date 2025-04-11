#pragma once

#include "../../macro.h"
#include <type_traits>

GDF_NS_START

template<typename T>
concept LegallyBindable = 
  requires(T one, T two) {
    one == two;
  };

template<class FT1, class FT2>
bool operator==(std::function<FT1> const& oat, std::function<FT2> const& meal) {
  if (oat.target_type() != meal.target_type()) return false;
  return oat.template target<FT1>() == meal.template target<FT1>();
}

template<LegallyBindable T>
/// A struct that allows you to listen to its state's change.
///
/// When the value changes, you can access the object's old value before all the callbacks are processed.
struct Bindable {
private:
  T m_value;
  std::vector<std::function<void(T)>> m_listeners;
public:
  Bindable() : m_value(T()) {}
  Bindable(T value) : m_value(value) {};

  operator T () {return m_value;}

  Bindable<T>& operator=(const T& other) {
    if (m_value != other) {
      for (auto& l : m_listeners) {
        l(other);
      }
      m_value = other;
    }
    return *this;
  }
  Bindable<T>& operator=(const Bindable<T>& other) {
    return this->operator=(other.m_value);
  }
  void addCallback(std::function<void(T)> listener) {
    m_listeners.push_back(listener);
  }
  T& operator->() {return m_value;}
  T& get() {return m_value;}
  void removeCallback(std::function<void(T)> listener) {
    m_listeners.erase(std::remove_if(m_listeners.begin(), m_listeners.end(), [listener](auto& l) {
      return l == listener;
    }), m_listeners.end());
  }
};

GDF_NS_END
