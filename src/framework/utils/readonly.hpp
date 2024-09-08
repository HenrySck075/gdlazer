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

    T& inner() {return m_t;}

    // how does this work
    friend bool operator==(const readonly<T,OWNER>& lhs, const T& rhs) {return lhs.m_t == rhs;}
    friend bool operator==(const readonly<T,OWNER>& lhs, const readonly<T,OWNER>& rhs) {return lhs.m_t == rhs.m_t;}

private:
    T& operator =(const T &t) {
        m_t = t; 
        return m_t;
    }

    T m_t;
};