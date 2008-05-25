/*
  bbase : version 0.14 ~byuu (2008-04-16)
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
typedef unsigned int uint;

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
  #define mkdir     _mkdir
  #define putenv    _putenv
  #define rmdir     _rmdir
  #define vsnprintf _vsnprintf
  #define usleep(n) Sleep(n / 1000)

  static char *realpath(const char *file_name, char *resolved_name) {
    return _fullpath(resolved_name, file_name, PATH_MAX);
  }
#else
  #define mkdir(path) (mkdir)(path, 0755);
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

/*****
 * OS localization
 *****/

//userpath(output) retrieves path to user's home folder
//output must be at least as large as PATH_MAX

#if defined(_MSC_VER) || defined(__MINGW32__)
static char *userpath(char *output) {
  strcpy(output, "."); //failsafe
  SHGetFolderPath(0, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 0, 0, output);
  return output;
}
#else
static char *userpath(char *output) {
  strcpy(output, "."); //failsafe
  struct passwd *userinfo = getpwuid(getuid());
  if(userinfo) { strcpy(output, userinfo->pw_dir); }
  return output;
}
#endif

template<int min, int max, typename T> inline T minmax(const T x) {
  return (x < (T)min) ? (T)min : (x > (T)max) ? (T)max : x;
}

/*****
 * endian wrappers
 *****/

#ifndef ARCH_MSB
//little-endian: uint8[] { 0x01, 0x02, 0x03, 0x04 } == 0x04030201
  #define order_lsb2(a,b)             a,b
  #define order_lsb3(a,b,c)           a,b,c
  #define order_lsb4(a,b,c,d)         a,b,c,d
  #define order_lsb5(a,b,c,d,e)       a,b,c,d,e
  #define order_lsb6(a,b,c,d,e,f)     a,b,c,d,e,f
  #define order_lsb7(a,b,c,d,e,f,g)   a,b,c,d,e,f,g
  #define order_lsb8(a,b,c,d,e,f,g,h) a,b,c,d,e,f,g,h
  #define order_msb2(a,b)             b,a
  #define order_msb3(a,b,c)           c,b,a
  #define order_msb4(a,b,c,d)         d,c,b,a
  #define order_msb5(a,b,c,d,e)       e,d,c,b,a
  #define order_msb6(a,b,c,d,e,f)     f,e,d,c,b,a
  #define order_msb7(a,b,c,d,e,f,g)   g,f,e,d,c,b,a
  #define order_msb8(a,b,c,d,e,f,g,h) h,g,f,e,d,c,b,a
#else
//big-endian:    uint8[] { 0x01, 0x02, 0x03, 0x04 } == 0x01020304
  #define order_lsb2(a,b)             b,a
  #define order_lsb3(a,b,c)           c,b,a
  #define order_lsb4(a,b,c,d)         d,c,b,a
  #define order_lsb5(a,b,c,d,e)       e,d,c,b,a
  #define order_lsb6(a,b,c,d,e,f)     f,e,d,c,b,a
  #define order_lsb7(a,b,c,d,e,f,g)   g,f,e,d,c,b,a
  #define order_lsb8(a,b,c,d,e,f,g,h) h,g,f,e,d,c,b,a
  #define order_msb2(a,b)             a,b
  #define order_msb3(a,b,c)           a,b,c
  #define order_msb4(a,b,c,d)         a,b,c,d
  #define order_msb5(a,b,c,d,e)       a,b,c,d,e
  #define order_msb6(a,b,c,d,e,f)     a,b,c,d,e,f
  #define order_msb7(a,b,c,d,e,f,g)   a,b,c,d,e,f,g
  #define order_msb8(a,b,c,d,e,f,g,h) a,b,c,d,e,f,g,h
#endif

/*****
 * libc extensions
 *****/

//pseudo-random number generator
static unsigned prng() {
  static unsigned n = 0;
  return n = (n >> 1) ^ (((n & 1) - 1) & 0xedb88320);
}

static uint64 fget(FILE *fp, unsigned length = 1) {
  uint64 data = 0;
  for(unsigned i = 0; i < length; i++) {
    data |= fgetc(fp) << (i << 3);
  }
  return data;
}

static void fput(FILE *fp, uint64 data, unsigned length = 1) {
  for(unsigned i = 0; i < length; i++) {
    fputc(data >> (i << 3), fp);
  }
}

static bool fexists(const char *fn) {
  FILE *fp = fopen(fn, "rb");
  if(!fp) return false;
  fclose(fp);
  fp = 0;
  return true;
}

static unsigned fsize(FILE *fp) {
  if(!fp) return 0;
  unsigned pos = ftell(fp);
  fseek(fp, 0, SEEK_END);
  unsigned size = ftell(fp);
  fseek(fp, pos, SEEK_SET);
  return size;
}

static unsigned fsize(const char *fn) {
  FILE *fp = fopen(fn, "rb");
  if(!fp) return 0;
  fseek(fp, 0, SEEK_END);
  unsigned size = ftell(fp);
  fclose(fp);
  fp = 0;
  return size;
}

static int fresize(FILE *fp, long size) {
  return ftruncate(fileno(fp), size);
}

#endif //ifndef BBASE_H
