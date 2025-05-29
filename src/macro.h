#pragma once

#define GDF_NS_START namespace gdlazer { namespace frameworks {
#define GDF_NS_END }}
#define GDF_NS gdlazer::frameworks
#define GDL_NS_START namespace gdlazer { namespace game {
#define GDL_NS_END }}
#define GDL_NS gdlazer::game

#define breakpoint()

#ifdef __GNUC__
  #define noinline __attribute__((noinline))
#else 
  #define noinline __declspec(noinline) 
#endif

#ifdef GEODE_IS_WINDOWS
  #define breakpoint() if (IsDebuggerPresent()) DebugBreak()
#endif

#define $createClass(classname, initfunc, ...) \
  classname* ret = new classname();      \
  if (ret && ret->initfunc(__VA_ARGS__)) {   \
    ret->autorelease();            \
  } else {                   \
    delete ret;        \
    return nullptr; \
  };                     \
  return ret

#define $defaultCreate(classname) \
  static noinline classname* create() {  \
  $createClass(classname, init);\
  }

#if defined(__GNUC__) || defined(__clang__)
  #define $verifyPtr(...) \
  ({ \
    auto GEODE_CONCAT(res,__LINE__) = __VA_ARGS__; \
    if (!GEODE_CONCAT(res,__LINE__)) return false; \
    GEODE_CONCAT(res,__LINE__); \
  })
  
#else
  #error "we banned msvc unfortunately, go use clang"
#endif
