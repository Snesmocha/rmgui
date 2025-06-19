#ifndef UTIL_H
#define UTIL_H




#if defined(__GNUC__) || defined(__clang__)

#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#define FORCE_INLINE __attribute__((always_inline)) inline

#define NO_THROW __attribute__((__nothrow__))
#define LEAF __attribute__((__leaf__))


#elif defined(_MSC_VER)   /*MSVC is the only crappy compiler that doesnt support this*/

#define UNLIKELY(x) (x)
#define LIKELY(x) (x)

#define FORCE_INLINE __forceinline

#define NO_THROW __declspec(nothrow)
#define LEAF

#else

#define UNLIKELY(x) (x)
#define LIKELY(x) (x)

#define FORCE_INLINE

#define NO_THROW
#define LEAF

#endif



#endif
