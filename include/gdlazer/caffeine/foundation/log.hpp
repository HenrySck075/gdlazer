#pragma once

#include "Geode/loader/Event.hpp"
#include "gdlazer/caffeine/foundation/utils/readonly.hpp"
#include <Geode/loader/Log.hpp>
#include <typeindex>

struct godelog {
  void* owner;
  template<typename T>
  std::string_view getObjectName(T* obj) {
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
  template <typename... Args>
  inline void logImpl(geode::Severity severity, geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    geode::log::logImpl(severity, geode::getMod(), "[{}]: {}", getObjectName(owner), fmt::vformat(str, std::forward(args...)));
  }
  template <typename... Args>
  inline void debug(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
      logImpl(geode::Severity::Debug, str, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline void info(impl::FmtStr<Args...> str, Args&&... args) {
      logImpl(Severity::Info, getMod(), str, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline void warn(impl::FmtStr<Args...> str, Args&&... args) {
      logImpl(Severity::Warning, getMod(), str, std::forward<Args>(args)...);
  }

  template <typename... Args>
  inline void error(impl::FmtStr<Args...> str, Args&&... args) {
      logImpl(Severity::Error, getMod(), str, std::forward<Args>(args)...);
  }
};

class WithLogger {
public:
  readonly<godelog, WithLogger> m_log;
  WithLogger() : m_log({this}) {};
};
