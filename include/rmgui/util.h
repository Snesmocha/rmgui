#ifndef UTIL_H
#define UTIL_H

/* put this in some utilities header or something */
#if defined(__GNUC__) || defined(__clang__)

#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#define FORCE_INLINE __attribute__((always_inline)) inline


#elif defined(_MSC_VER)   /*MSVC is the only crappy compiler that doesnt support this*/

#define UNLIKELY(x) (x)
#define LIKELY(x) (x)

#define FORCE_INLINE __forceinline

#else

#define UNLIKELY(x) (x)
#define LIKELY(x) (x)

#define FORCE_INLINE

#endif

#endif
