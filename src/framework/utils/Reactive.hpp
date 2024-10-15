#pragma once
// i dont have internet access
// @note Part of CocosUtils

#include <Geode/Geode.hpp>
using namespace geode::prelude;

template<typename T>
class Reactive {
    T _value;
    using Callback = std::function<void(T,T)>;
    /// void callback(T old, T new)
    std::vector<Callback> callbacks;
public:
    Reactive() : _value(T()) {}
    Reactive(T value) : _value(value) {
        if (auto ccvalue = typeinfo_cast<CCObject*>(value)) ccvalue->retain();
    }
    ~Reactive() {
        if (auto ccvalue = typeinfo_cast<CCObject*>(_value)) ccvalue->release();
    }

    T operator=(T value) {
        if (auto ccvalue = typeinfo_cast<CCObject*>(value)) ccvalue->retain();
        for (auto& i : callbacks) i(_value,value);
        if (auto ccvalue = typeinfo_cast<CCObject*>(_value)) ccvalue->release();
        _value = value; 
    }

    operator T() {return _value;}
    T operator->() {return _value;}

    void addCallback(Callback const& cb) {callbacks.push_back(cb);};
    void removeCallback(Callback const& cb) {callbacks.erase(cb);}
};
