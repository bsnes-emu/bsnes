#ifndef NALL_STDINT_HPP
#define NALL_STDINT_HPP

#if defined(_MSC_VER)
  typedef signed char int8_t;
  typedef signed short int16_t;
  typedef signed int int32_t;
  typedef signed long long int64_t;
  typedef int64_t intmax_t;
  #if defined(_WIN64)
  typedef int64_t intptr_t;
  #else
  typedef int32_t intptr_t;
  #endif

  typedef unsigned char uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned int uint32_t;
  typedef unsigned long long uint64_t;
  typedef uint64_t uintmax_t;
  #if defined(_WIN64)
  typedef uint64_t uintptr_t;
  #else
  typedef uint32_t uintptr_t;
  #endif
#else
  #include <stdint.h>
#endif

#if defined(__SIZEOF_INT128__)
  using int128_t = signed __int128;
  using uint128_t = unsigned __int128;
#endif

using float32_t = float;
using float64_t = double;
using float80_t = long double;

static_assert(sizeof(int8_t)   == 1, "int8_t is not of the correct size" );
static_assert(sizeof(int16_t)  == 2, "int16_t is not of the correct size");
static_assert(sizeof(int32_t)  == 4, "int32_t is not of the correct size");
static_assert(sizeof(int64_t)  == 8, "int64_t is not of the correct size");

static_assert(sizeof(uint8_t)  == 1, "int8_t is not of the correct size" );
static_assert(sizeof(uint16_t) == 2, "int16_t is not of the correct size");
static_assert(sizeof(uint32_t) == 4, "int32_t is not of the correct size");
static_assert(sizeof(uint64_t) == 8, "int64_t is not of the correct size");

static_assert(sizeof(float)       >=  4, "float32_t is not of the correct size");
static_assert(sizeof(double)      >=  8, "float64_t is not of the correct size");
static_assert(sizeof(long double) >= 10, "float80_t is not of the correct size");

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using intmax = intmax_t;
using intptr = intptr_t;

using uint = unsigned int;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using uintmax = uintmax_t;
using uintptr = uintptr_t;

#if defined(__SIZEOF_INT128__)
using int128 = int128_t;
using uint128 = uint128_t;
#endif

#endif
