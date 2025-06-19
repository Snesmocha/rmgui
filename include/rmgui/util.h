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


FORCE_INLINE size_t round_next_pow2(size_t number)
{
    if(number < 2)
    {
        return 1;
    }
#if defined(__GNUC__) || defined(__CLANG__)

#if SIZE_MAX > 0xFFFFFFFF
    #define builtin_clz(x) __builtin_clzll(x)
#else
    #define builtin_clz(x) __builtin_clz(x)
#endif

    //count the leading bits to figure out the number of bits needed to shift 0x1 to get nearest pow
    return (size_t)0x1 << ((sizeof(size_t) * 8) - (size_t)builtin_clz(--number));
#elif defined(__MSC_VER_)

#if SIZE_MAX > 0xFFFFFFFF
    #define bitscan_reverse(x, y) _BitScanReverse64(x, y)
#else
    #define bitscan_reverse(x, y) _BitScanReverse(x, y)
#endif

    unsigned long idx;
    bitscan_reverse(&idx, --number);

    return (size_t)0x1 << (idx + 1);

#else
    number--;
    number |= number >> 1;
    number |= number >> 2;
    number |= number >> 4;
    number |= number >> 8;
    number |= number >> 16;
#endif    
#if SIZE_MAX > 0xFFFFFFFF
    number |= number >> 32;
#endif
    number++;

    return number;
}



#ifdef __cplusplus
}
#endif



#endif
