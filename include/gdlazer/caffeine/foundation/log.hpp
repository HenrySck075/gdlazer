#pragma once

#include <Geode/loader/Log.hpp>
#include <typeindex>


namespace caffeine { 

  namespace log {

  template<typename T>
  static std::string_view getObjectName(T* obj) {
#ifdef GEODE_IS_WINDOWS
    std::string_view tname = typeid(*obj).name();
    if (tname.starts_with("class ")) {
        tname.remove_prefix(6);
    } else if (tname.starts_with("struct ")) {
        tname.remove_prefix(7);
    }

    return tname;
#else
    static std::unordered_map<std::type_index, std::string> s_typeNames;
    std::type_index key = typeid(*obj);

    auto it = s_typeNames.find(key);
    if (it != s_typeNames.end()) {
        return it->second;
    }

    std::string ret;

    int status = 0;
    auto demangle = abi::__cxa_demangle(typeid(*obj).name(), 0, 0, &status);
    if (status == 0) {
        ret = demangle;
    }
    free(demangle);
    auto [iter, _] = s_typeNames.insert({key, std::move(ret)});

    return iter->second;
#endif
  }

  class StringConvertible {
  public:
    std::string toString() {return "<" + std::string(getObjectName(this)) + ">";}
  };

  inline std::string format_as(StringConvertible* po) {
    return po->toString();
  };

  // calls geode::log::pushNest() on ctor and geode::log::popNest() on dtor
  struct LogNestPusher {
    LogNestPusher() {
      geode::log::pushNest();
    }
    ~LogNestPusher() {
      geode::log::popNest();
    }
  };
}}
