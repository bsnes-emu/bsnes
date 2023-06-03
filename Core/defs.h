#ifndef defs_h
#define defs_h

#define GB_likely(x)   __builtin_expect((bool)(x), 1)
#define GB_unlikely(x) __builtin_expect((bool)(x), 0)

#ifdef GB_INTERNAL

// "Keyword" definitions
#define likely(x)   GB_likely(x)
#define unlikely(x) GB_unlikely(x)

#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8)
#define __builtin_bswap16(x) ({ typeof(x) _x = (x); _x >> 8 | _x << 8; })
#endif

#define internal __attribute__((visibility("internal")))
#define noinline __attribute__((noinline))

#if __clang__
#define unrolled _Pragma("unroll")
#define nounroll _Pragma("clang loop unroll(disable)")
#elif __GNUC__ >= 8
#define unrolled _Pragma("GCC unroll 8")
#define nounroll _Pragma("GCC unroll 0")
#else
#define unrolled
#define nounroll
#endif

#define unreachable() __builtin_unreachable();
#define nodefault default: unreachable()

#ifdef GB_BIG_ENDIAN
#define LE16(x) __builtin_bswap16(x)
#define LE32(x) __builtin_bswap32(x)
#define LE64(x) __builtin_bswap64(x)
#define BE16(x) (x)
#define BE32(x) (x)
#define BE64(x) (x)
#else
#define LE16(x) (x)
#define LE32(x) (x)
#define LE64(x) (x)
#define BE16(x) __builtin_bswap16(x)
#define BE32(x) __builtin_bswap32(x)
#define BE64(x) __builtin_bswap64(x)
#endif
#endif

struct GB_gameboy_s;
typedef struct GB_gameboy_s GB_gameboy_t;
#endif 
