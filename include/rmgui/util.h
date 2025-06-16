#ifndef UTIL_H
#define UTIL_H

/* put this in some utilities header or something */
#if defined(__GNUC__) || defined(__clang__)

#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#else   /*MSVC is the only crappy compiler that doesnt support this*/

#define UNLIKELY(x) (x)
#define LIKELY(x) (x)

#endif

#endif