#pragma once

// A specialized struct for read-only public access, and read-write private access (to a specific class)
template<typename T, class Owner>
class readonly {
private:
  T value;
public:
  readonly() = default;
  readonly(const T& v) : value(v) {}
  // Read-only access
  operator const T&() const { return value; }
  const T* operator->() const { return &value; }
  const T& get() const { return value; }
  // Allow Owner class to modify the value
  friend Owner;
};
