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

#define $create_class(classname, initfunc, ...) \
  classname* ret = new classname();      \
  if (ret && ret->initfunc(__VA_ARGS__)) {   \
    ret->autorelease();            \
  } else {                   \
    CC_SAFE_RELEASE_NULL(ret);         \
  };                     \
  return ret

#define $default_create(classname) \
  static noinline classname* create() {  \
  $create_class(classname, init);\
  }
