#pragma once


#include "../../macro.h"
#include <Geode/loader/Log.hpp>
GDF_NS_START

/// Another layer of Geode logger (no-op on release because it's just not worth it)
class Logger {
private:
  std::string m_name;
  geode::Severity m_severity;

public:
  Logger(std::string name, geode::Severity severity = geode::Severity::Info)
  : m_name(name), m_severity(severity) {}
  
  inline void setSeverity(geode::Severity severity) {
    m_severity = severity;
  }
  inline geode::Severity getSeverity() const {
    return m_severity;
  }
  inline void setName(std::string name) {
    m_name = std::move(name);
  }
  inline std::string getName() const {
    return m_name;
  }

  template<typename ...Args>
  inline void logImpl(geode::Severity severity, geode::log::impl::FmtStr<Args...> str, Args&& ...args) {
    #ifdef GDF_DEBUG
    if (severity >= m_severity) geode::log::logImpl(severity, geode::Mod::get(), "[{}]: {}", m_name, fmt::format(str, std::forward<Args>(args)...));
    #endif
  }

  template <typename... Args>
  inline void debug(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    #ifdef GDF_DEBUG
    logImpl(geode::Severity::Debug, str, std::forward<Args>(args)...);
    #endif
  }

  template <typename... Args>
  inline void info(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    #ifdef GDF_DEBUG
    logImpl(geode::Severity::Info, str, std::forward<Args>(args)...);
    #endif
  }

  template <typename... Args>
  inline void warn(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    #ifdef GDF_DEBUG
    logImpl(geode::Severity::Warning, str, std::forward<Args>(args)...);
    #endif
  }

  template <typename... Args>
  inline void error(geode::log::impl::FmtStr<Args...> str, Args&&... args) {
    #ifdef GDF_DEBUG
    logImpl(geode::Severity::Error, str, std::forward<Args>(args)...);
    #endif
  }

};

GDF_NS_END