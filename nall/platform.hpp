#ifndef NALL_PLATFORM_HPP
#define NALL_PLATFORM_HPP

#include <nall/utf8.hpp>

//=========================
//standard platform headers
//=========================

#include <limits>

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
  #include <io.h>
  #include <direct.h>
  #include <shlobj.h>
  #undef interface
  #define dllexport __declspec(dllexport)
#else
  #include <unistd.h>
  #include <pwd.h>
  #include <sys/stat.h>
  #define dllexport
#endif

//==================
//warning supression
//==================

//Visual C++
#if defined(_MSC_VER)
  //disable libc "deprecation" warnings
  #pragma warning(disable:4996)
#endif

//================
//POSIX compliance
//================

#if defined(_MSC_VER)
  #define PATH_MAX  _MAX_PATH
  #define va_copy(dest, src)  ((dest) = (src))
#endif

#if defined(_WIN32)
  #define getcwd      _getcwd
  #define ftruncate   _chsize
  #define putenv      _putenv
  #define mkdir(n, m) _wmkdir(nall::utf16_t(n))
  #define rmdir       _rmdir
  #define vsnprintf   _vsnprintf
  #define usleep(n)   Sleep(n / 1000)
#endif

//================
//inline expansion
//================

#if defined(__GNUC__)
  #define noinline      __attribute__((noinline))
  #define inline        inline
  #define alwaysinline  inline __attribute__((always_inline))
#elif defined(_MSC_VER)
  #define noinline      __declspec(noinline)
  #define inline        inline
  #define alwaysinline  inline __forceinline
#else
  #define noinline
  #define inline        inline
  #define alwaysinline  inline
#endif

#endif

