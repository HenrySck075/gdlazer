#if 1
#define GDF_EXCEPTABLE(...) std::expected<__VA_ARGS__, std::exception>
#define GDF_THROW(...) return std::unexpected(__VA_ARGS__)
#define GDF_EXC_TRY { \
  auto&& result = ([&](){
#define GDF_EXC_ETRY })();
#define GDF_EXC_TRY_OPERATE(...) ({ \
    auto&& __result = (__VA_ARGS__); \
    if (!__result.has_value()) { \
        return std::unexpected(__result.error()); \
    } \
    __result.value(); \
})
#define GDF_EXC_EXCEPT(type, var) })(); \
  if (!result.has_value()) { \
    ([&](){type var = static_cast<type>(result.error());
#define GDF_EXC_FINALLY })(); } ([&](){ \

#else
#define GDF_EXCEPTABLE(...) __VA_ARGS__
#define GDF_THROW(...) throw __VA_ARGS__
#define GDF_EXC_TRY try {
#define GDF_EXC_ETRY }
#define GDF_EXC_TRY_OPERATE(...) __VA_ARGS__
#define GDF_EXC_EXCEPT(...) } catch (__VA_ARGS__) {
#define GDF_EXC_FINALLY } finally {
#endif


#define finally catch(std::exception&)



#define $mixin template<class Derived> class
#define $mxself Derived* self = static_cast<Derived*>(this)
