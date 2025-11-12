#pragma once

class Key {
protected:
  virtual void __abstractFlag() = 0;
};


class LocalKey : public Key {};

template<typename T>
class ObjectKey : public LocalKey {
protected:
  void __abstractFlag() override {};
  T m_object;
public:
  ObjectKey(T& object) : m_object(object) {};
};
