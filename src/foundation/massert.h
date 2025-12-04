#if GDF_DEBUG
#undef NDEBUG
#include <assert.h>
#endif
#define massert(expr, msg) assert(( (void)(msg), (expr) ))
