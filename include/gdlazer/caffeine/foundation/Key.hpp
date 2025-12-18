#pragma once
#include <gdlazer/caffeine/foundation/utils/Ref.hpp>

#include <memory>


namespace caffeine {
class Key : public cocos2d::CCObject {
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

/// GlobalKey lore


/// The untemplated version of GlobalKey for use in member types
class GlobalKeyU : public Key {
protected:
  RefNauseam<Widget> m_currentWidget;
  RefNauseam<BuildContext> m_currentContext;
  friend class Element;
  friend class BuildOwner;
};
template<typename T>
class GlobalKey : public GlobalKeyU {
protected:
  std::shared_ptr<T> m_currentState;
  friend class Element; 
};

}
