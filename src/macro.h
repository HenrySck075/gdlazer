#pragma once

#include <Geode/DefaultInclude.hpp>

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
  if (ret && ret->initfunc(__VA_ARGS__)) {\
    ret->autorelease();                    \
  } else {                                  \
    delete ret;                              \
    return nullptr;                           \
  };                                           \
  return ret

#define $createClass2_2(classname, initfunc) $createClass(classname, initfunc, args)
#define $createClass2_1(classname) $createClass2_2(classname, init)
#define $createClass2(...) GEODE_INVOKE(GEODE_CONCAT($createClass2_, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)

#define $defaultCreate(classname)       \
  static noinline classname* create() {  \
    $createClass(classname, init);        \
  }

#if defined(__GNUC__) || defined(__clang__)
  #define $verifyPtr(...) \
  ({ \
    auto GEODE_CONCAT(res,__LINE__) = __VA_ARGS__; \
    if (!GEODE_CONCAT(res,__LINE__)) return false;  \
    GEODE_CONCAT(res,__LINE__); \
  })
  
#else
  #error "we banned msvc unfortunately, go use clang"
#endif

#define PAREN ()
/// Macro for joining __VA_ARGS__ used in GDF_KWARGS_STRUCT with a ; as the separator
#define _$structArgsCreatorReal(...) __VA_OPT__(GEODE_EXPAND(_$structArgsCreatorH(__VA_ARGS__)))
#define _$structArgsCreatorH(v, ...) \
  decltype(GEODE_CONCAT(m_, v)) v; \
  __VA_OPT__(_$structArgsCreatorR PAREN (__VA_ARGS__))
#define _$structArgsCreatorR() _$structArgsCreatorH
#define _$structArgsCreator(...) GEODE_EXPAND(_$structArgsCreatorReal(__VA_ARGS__))


#define _$structArgsInitializerReal(...) __VA_OPT__(GEODE_EXPAND(_$structArgsInitializerH(__VA_ARGS__)))
#define _$structArgsInitializerH(v, ...) \
  this->GEODE_CONCAT(m_, v) = args.v; \
  __VA_OPT__(_$structArgsInitializerR PAREN (__VA_ARGS__))
#define _$structArgsInitializerR() _$structArgsInitializerH
#define _$structArgsInitializer(...) GEODE_EXPAND(_$structArgsInitializerReal(__VA_ARGS__))

#define GDF_KWARGS ConstructorKwargs args
#define GDF_KWARGS_STRUCT(...) struct ConstructorKwargs { \
  _$structArgsCreator(__VA_ARGS__) \
}; \
void ctorInitArgs(GDF_KWARGS) {\
  _$structArgsInitializer(__VA_ARGS__)\
}