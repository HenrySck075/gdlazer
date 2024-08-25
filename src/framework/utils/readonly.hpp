// mrow
//
// stolen code but meh
#pragma once

template <class T, class OWNER>
class readonly {
    friend OWNER;

public:
    explicit readonly(const T &t) : m_t(t) {
    }

    ~readonly() {
    }

    operator const T&() const {
        return m_t;
    }

private:
    T& operator =(const T &t) {
        m_t = t; 
        return m_t;
    }

    T m_t;
};