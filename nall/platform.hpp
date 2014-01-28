#ifndef NALL_PLATFORM_HPP
#define NALL_PLATFORM_HPP

namespace Math {
  static const long double e  = 2.71828182845904523536;
  static const long double Pi = 3.14159265358979323846;
}

#if defined(_WIN32)
  //minimum version needed for _wstat64, etc
  #undef  __MSVCRT_VERSION__
  #define __MSVCRT_VERSION__ 0x0601
  #include <nall/windows/utf8.hpp>
#endif

//=========================
//standard platform headers
//=========================

#include <limits>
#include <utility>

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32)
  #include <io.h>
  #include <direct.h>
  #include <shlobj.h>
  #include <wchar.h>
  #undef interface
  #define dllexport __declspec(dllexport)
#else
  #include <dlfcn.h>
  #include <unistd.h>
  #include <pwd.h>
  #define dllexport
#endif

//==========
//Visual C++
//==========

#if defined(_MSC_VER)
  #pragma warning(disable:4996)  //disable libc "deprecation" warnings
  #define va_copy(dest, src) ((dest) = (src))
#endif

#if defined(_WIN32)
  __declspec(dllimport) int _fileno(FILE*);

  inline int access(const char* path, int amode) { return _waccess(nall::utf16_t(path), amode); }
  inline int fileno(FILE* stream) { return _fileno(stream); }
  inline char* getcwd(char* buf, size_t size) { wchar_t wpath[PATH_MAX] = L""; if(!_wgetcwd(wpath, size)) return nullptr; strcpy(buf, nall::utf8_t(wpath)); return buf; }
  inline int putenv(char* string) { return _wputenv(nall::utf16_t(string)); }
  inline char* realpath(const char* file_name, char* resolved_name) { wchar_t wfile_name[PATH_MAX] = L""; if(!_wfullpath(wfile_name, nall::utf16_t(file_name), PATH_MAX)) return nullptr; strcpy(resolved_name, nall::utf8_t(wfile_name)); return resolved_name; }
  inline int rename(const char* oldname, const char* newname) { return _wrename(nall::utf16_t(oldname), nall::utf16_t(newname)); }
  inline void usleep(unsigned milliseconds) { Sleep(milliseconds / 1000); }
#endif

//================
//inline expansion
//================

#if defined(__clang__) || defined(__GNUC__)
  #define noinline      __attribute__((noinline))
  #define alwaysinline  inline __attribute__((always_inline))
#elif defined(_MSC_VER)
  #define noinline      __declspec(noinline)
  #define alwaysinline  inline __forceinline
#else
  #define noinline
  #define alwaysinline  inline
#endif

//===========
//unreachable
//===========

#if defined(__clang__) || defined(__GNUC__)
  #define unreachable __builtin_unreachable()
#else
  #define unreachable throw
#endif

#endif
