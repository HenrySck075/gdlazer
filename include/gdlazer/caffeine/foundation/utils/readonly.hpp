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

  // Comparisons
  inline bool operator==(const T& other) const { return value == other; }
  inline bool operator<(const T& other) const { return value < other; }
  
  // (with other readonly object)
  inline bool operator==(const readonly<T, Owner>& other) const { return value == other.value; }
  inline bool operator<(const readonly<T, Owner>& other) const { return value < other.value; }

  

};
