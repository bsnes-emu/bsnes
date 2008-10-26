/*
  bbase : version 0.17 ~byuu (2008-10-19)
  license: public domain
*/

#ifndef BBASE_H
#define BBASE_H

#include <nall/stdint.hpp>
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef unsigned uint;

#include <algorithm>
using std::min;
using std::max;

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>

#if defined(_MSC_VER) || defined(__MINGW32__)
  #include <io.h>
  #include <direct.h>
  #include <shlobj.h>
#else
  #include <unistd.h>
  #include <pwd.h>
  #include <sys/stat.h>
#endif

#if defined(_MSC_VER)
  //disable libc deprecation warnings in MSVC 2k5+
  #pragma warning(disable:4996)

  #define NOMINMAX
  #define PATH_MAX  _MAX_PATH
  #define va_copy(dst, src) ((dst) = (src))
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
  #define getcwd    _getcwd
  #define ftruncate _chsize
  #define putenv    _putenv
  #define rmdir     _rmdir
  #define vsnprintf _vsnprintf
  #define usleep(n) Sleep(n / 1000)
#endif

/*****
 * inline expansion
 *****/

#if defined(_MSC_VER)
  #define noinline      __declspec(noinline)
  #define inline        inline
  #define alwaysinline  __forceinline
#elif defined(__GNUC__)
  #define noinline      __attribute__((noinline))
  #define inline        inline
  #define alwaysinline  __attribute__((always_inline))
#else
  #define noinline
  #define inline        inline
  #define alwaysinline  inline
#endif

#endif //ifndef BBASE_H
