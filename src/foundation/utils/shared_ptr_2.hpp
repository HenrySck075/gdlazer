#pragma once

#include <cstddef>
template<typename T>
struct shared_ptr_ctor {
private:
    std::shared_ptr<T> m_ptr;
public:
    shared_ptr_ctor() : m_ptr(std::make_shared<T>()) {}

    template<typename... Args>
    shared_ptr_ctor(Args&&... args) : m_ptr(std::make_shared<T>(std::forward<Args>(args)...)) {}

    shared_ptr_ctor(T* ptr) : m_ptr(std::shared_ptr<T>(ptr)) {}
    shared_ptr_ctor(shared_ptr_ctor<T>& me) : m_ptr(me.m_ptr) {};

    operator std::shared_ptr<T>() const {
        return m_ptr;
    }
    operator std::shared_ptr<T>() {
      return m_ptr;
    }

    T* operator->() {
        return m_ptr.get();
    }
    T* operator->() const {
        return m_ptr.get();
    }

    T& operator*() {
        return *m_ptr;
    }
    const T& operator*() const {
        return *m_ptr;
    }

    T* get() {
        return m_ptr;
    }

    T* get() const {
        return m_ptr;
    }

    bool operator==(std::nullptr_t) {
      return m_ptr == nullptr;
    }
    bool operator==(std::shared_ptr<T>& m) {
      return m.get() == m_ptr.get();
    }
};
