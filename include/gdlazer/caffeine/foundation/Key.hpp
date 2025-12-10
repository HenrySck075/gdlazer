#pragma once

#include <memory>
class Key {
public:
  virtual bool equals(std::shared_ptr<Key> other) = 0;
};


/// LocalKey lore
class LocalKey : public Key {};

template<typename T>
/// A key that takes its identity from the object used as its value.
///
/// Also doubles as a ValueKey<T> if you think about it
class ObjectKey : public LocalKey {
protected:
  T m_object;
public:
  ObjectKey(T& object) : m_object(object) {};

  bool equals(const std::shared_ptr<Key> other) override {
    if (auto otter = dynamic_cast<ObjectKey<T>>(other)) {
      return &m_object == &otter->m_object;
    }
    return false;
  }
};

/// A key that only equals to itself.
class UniqueKey : public LocalKey {
  bool equals(const std::shared_ptr<Key> other) override {
    return this == other.get();
  }
};

class Widget;
class BuildContext;

class GlobalKeyU : public Key {
protected:
  std::shared_ptr<Widget> m_currentWidget;
  std::shared_ptr<BuildContext> m_currentContext;
  friend class Element;
  friend class BuildOwner;
};
/// GlobalKey lore
template<typename T>
class GlobalKey : public GlobalKeyU {
protected:
  std::shared_ptr<T> m_currentState;
  friend class Element; 
};
