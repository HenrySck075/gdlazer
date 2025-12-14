#pragma once

#include "Geode/loader/Event.hpp"
#include "gdlazer/caffeine/foundation/utils/readonly.hpp"
#include <Geode/loader/Log.hpp>
#include <typeindex>


namespace caffeine { namespace log {

  template<typename T>
  std::string format_as(T* po);

  namespace impl {
    template <class T>
    concept IsPointer = std::is_pointer_v<std::decay_t<T>>;

    template <class T>
    concept CFormattable = IsPointer<T> && requires(T ptr) { caffeine::log::format_as(ptr); };


    template <class T>
    inline decltype(auto) wrapPointerObj(T&& value) {
        if constexpr (IsPointer<T>) {
          if constexpr (CFormattable<T>) {
            return caffeine::log::format_as(value);
          } else {
            return geode::format_as(value);
          }
        } else {
            return std::forward<T>(value);
        }
    }
  }

  struct godelog {
    void* owner;
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
    template <typename... Args>
    inline void logImpl(geode::Severity severity, geode::log::impl::FmtStr<Args...> str, Args&&... args) const {
      geode::log::logImpl(severity, geode::getMod(), "[{}]: {}", getObjectName(owner), fmt::vformat(str,fmt::make_format_args(impl::wrapPointerObj(args)...)));
    }
    template <typename... Args>
    inline void debug(geode::log::impl::FmtStr<Args...> str, Args&&... args) const {
      logImpl(geode::Severity::Debug, str, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void info(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
      logImpl(geode::Severity::Info, str, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void warn(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
      logImpl(geode::Severity::Warning, str, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void error(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
      logImpl(geode::Severity::Error, str, std::forward<Args>(args)...);
    }
  };

  class StringConvertible {
  public:
    std::string toString() {return "<" + std::string(godelog::getObjectName(this)) + ">";}
  };

  template<>
  inline std::string format_as(StringConvertible* po) {
    return po->toString();
  };

  class WithLogger : public StringConvertible {
  public:
    readonly<godelog, WithLogger> m_log;
    WithLogger() : m_log({this}) {};
  };


}}