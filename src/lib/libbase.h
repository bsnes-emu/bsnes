/*
  libbase : version 0.08b ~byuu (07/30/06)
*/

#ifndef __LIBBASE
#define __LIBBASE

#if defined(_MSC_VER)
//disable libc deprecation warnings in MSVC 2k5+
  #pragma warning(disable:4996)
#endif

/*****
 * inline expansion
 *****/

#if defined(_MSC_VER)
  #define noinline     __declspec(noinline)
  #define inline       inline
  #define forceinline  __forceinline
  #define fastcall     __fastcall
#elif defined(__GNUC__)
  #define noinline
  #define inline       inline
  #define forceinline  inline
  #define fastcall     __attribute__((fastcall))
#else
  #error "unsupported compiler"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef TRUE
  #define TRUE !FALSE
#endif

#define SafeFree(__n)    if(__n) { free(__n);      __n = 0; }
#define SafeDelete(__n)  if(__n) { delete(__n);    __n = 0; }
#define SafeRelease(__n) if(__n) { __n->Release(); __n = 0; }

/*****
 * typedefs
 *****/

typedef unsigned int       uint;
typedef signed int         sint;

typedef unsigned char      byte;
typedef unsigned short     word;
typedef unsigned long      ulong;
typedef unsigned long long uquad;

typedef unsigned char      bool8;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned long      uint32;
typedef unsigned long long uint64;

typedef signed char        int8;
typedef signed short       int16;
typedef signed long        int32;
typedef signed long long   int64;

/*****
 * templates
 *****/

template<typename T> inline void swap(T &x, T &y) {
T z = x;
  x = y;
  y = z;
}

template<int min, int max, typename T> inline T minmax(const T x) {
  return (x < T(min)) ? T(min) : (x > T(max)) ? T(max) : x;
}

template<int bits> inline unsigned uclamp(const unsigned x) {
enum { m = (1 << bits) - 1 };
  return (x > m) ? m : x;
}

template<int bits> inline unsigned uclip(const unsigned x) {
enum { m = (1 << bits) - 1 };
  return (x & m);
}

template<int bits> inline signed sclamp(const signed x) {
enum { b = 1 << (bits - 1), m = (1 << (bits - 1)) - 1 };
  return (x > m) ? m : (x < -b) ? -b : x;
}

template<int bits> inline signed sclip(const signed x) {
enum { b = 1 << (bits - 1), m = (1 << (bits - 1)) - 1 };
  return (x & b) ? (x | ~m) : (x & m);
}

//requires compiler arithmetic shift right support
//c++ standard does not define whether >> is arithmetic or logical
//template<int bits> inline signed sclip(const signed x) {
//enum { s = sizeof(x) * 8 - bits };
//  return (x << s) >> s;
//}

template<int bits, typename T> inline T rol(const T x) {
enum { s = (sizeof(T) << 3) - bits };
  return (x << bits) | (x >> s);
}

template<int bits, typename T> inline T ror(const T x) {
enum { s = (sizeof(T) << 3) - bits };
  return (x >> bits) | (x << s);
}

template<int bits, typename T> inline T asl(const T x) {
  return (x << bits);
}

template<int bits, typename T> inline T asr(const T x) {
enum { h = 1 << ((sizeof(T) << 3) - 1) };
enum { m = ~((1 << ((sizeof(T) << 3) - bits)) - 1) };
  return (x >> bits) | ((0 - !!(x & h)) & m);
}

template<int bits, typename T> inline T lsl(const T x) {
  return (x << bits);
}

template<int bits, typename T> inline T lsr(const T x) {
enum { m = ((1 << ((sizeof(T) << 3) - bits)) - 1) };
  return (x >> bits) & m;
}

template<unsigned bits, typename base = uint> class uint_t {
private:
base data;

public:
  inline operator unsigned() const { return data; }
                       inline unsigned operator ++(int)       { base r = data; data = uclip<bits>(data + 1); return r; }
                       inline unsigned operator --(int)       { base r = data; data = uclip<bits>(data - 1); return r; }
                       inline unsigned operator ++()          { data = uclip<bits>(data  + 1); return data; }
                       inline unsigned operator --()          { data = uclip<bits>(data  - 1); return data; }
  template<typename T> inline unsigned operator  =(const T i) { data = uclip<bits>(i);         return data; }
  template<typename T> inline unsigned operator |=(const T i) { data = uclip<bits>(data  | i); return data; }
  template<typename T> inline unsigned operator ^=(const T i) { data = uclip<bits>(data  ^ i); return data; }
  template<typename T> inline unsigned operator &=(const T i) { data = uclip<bits>(data  & i); return data; }
  template<typename T> inline unsigned operator<<=(const T i) { data = uclip<bits>(data << i); return data; }
  template<typename T> inline unsigned operator>>=(const T i) { data = uclip<bits>(data >> i); return data; }
  template<typename T> inline unsigned operator +=(const T i) { data = uclip<bits>(data  + i); return data; }
  template<typename T> inline unsigned operator -=(const T i) { data = uclip<bits>(data  - i); return data; }
  template<typename T> inline unsigned operator *=(const T i) { data = uclip<bits>(data  * i); return data; }
  template<typename T> inline unsigned operator /=(const T i) { data = uclip<bits>(data  / i); return data; }
  template<typename T> inline unsigned operator %=(const T i) { data = uclip<bits>(data  % i); return data; }

  inline uint_t() : data(0) {}
  inline uint_t(const base i) : data(uclip<bits>(i)) {}
};

template<unsigned bits, typename base = int> class int_t {
private:
base data;

public:
  inline operator signed() const { return data; }
                       inline signed operator ++(int)       { base r = data; data = sclip<bits>(data + 1); return r; }
                       inline signed operator --(int)       { base r = data; data = sclip<bits>(data - 1); return r; }
                       inline signed operator ++()          { data = sclip<bits>(data  + 1); return data; }
                       inline signed operator --()          { data = sclip<bits>(data  - 1); return data; }
  template<typename T> inline signed operator  =(const T i) { data = sclip<bits>(i);         return data; }
  template<typename T> inline signed operator |=(const T i) { data = sclip<bits>(data  | i); return data; }
  template<typename T> inline signed operator ^=(const T i) { data = sclip<bits>(data  ^ i); return data; }
  template<typename T> inline signed operator &=(const T i) { data = sclip<bits>(data  & i); return data; }
  template<typename T> inline signed operator<<=(const T i) { data = sclip<bits>(data << i); return data; }
  template<typename T> inline signed operator>>=(const T i) { data = sclip<bits>(data >> i); return data; }
  template<typename T> inline signed operator +=(const T i) { data = sclip<bits>(data  + i); return data; }
  template<typename T> inline signed operator -=(const T i) { data = sclip<bits>(data  - i); return data; }
  template<typename T> inline signed operator *=(const T i) { data = sclip<bits>(data  * i); return data; }
  template<typename T> inline signed operator /=(const T i) { data = sclip<bits>(data  / i); return data; }
  template<typename T> inline signed operator %=(const T i) { data = sclip<bits>(data  % i); return data; }

  inline int_t() : data(0) {}
  inline int_t(const base i) : data(sclip<bits>(i)) {}
};

typedef uint_t<24>         uint24;
typedef  int_t<24>          int24;
typedef uint_t<48, uint64> uint48;
typedef  int_t<48,  int64>  int48;

/*****
 * endian wrappers
 *****/

#ifndef ARCH_MSB
//little-endian: uint8[] { 0x01, 0x02, 0x03, 0x04 } = 0x04030201
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
#define order_lsbit8(b0,b1,b2,b3,b4,b5,b6,b7) b7:1,b6:1,b5:1,b4:1,b3:1,b2:1,b1:1,b0:1
#define order_msbit8(b7,b6,b5,b4,b3,b2,b1,b0) b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1
#else
//big-endian:    uint8[] { 0x01, 0x02, 0x03, 0x04 } = 0x01020304
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
#define order_lsbit8(b7,b6,b5,b4,b3,b2,b1,b0) b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1
#define order_msbit8(b0,b1,b2,b3,b4,b5,b6,b7) b7:1,b6:1,b5:1,b4:1,b3:1,b2:1,b1:1,b0:1
#endif

/*****
 * libc extensions
 *****/

inline bool fexists(const char *fn) {
FILE *fp = fopen(fn, "rb");
  if(!fp)return false;
  fclose(fp);
  fp = 0;
  return true;
}

inline uint32 fsize(FILE *fp) {
  if(!fp)return 0;
uint32 pos = ftell(fp);
  fseek(fp, 0, SEEK_END);
uint32 size = ftell(fp);
  fseek(fp, pos, SEEK_SET);
  return size;
}

inline uint32 fsize(const char *fn) {
FILE *fp = fopen(fn, "rb");
  if(!fp)return 0;
  fseek(fp, 0, SEEK_END);
uint32 size = ftell(fp);
  fclose(fp);
  fp = 0;
  return size;
}

/*****
 * crc32 calculation
 * TODO: create libhash and move this code there
 *****/

const uint32 crc32_table[256] = {
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
  0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
  0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
  0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
  0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
  0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
  0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
  0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
  0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
  0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
  0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
  0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
  0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
  0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
  0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
  0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
  0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
  0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
  0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
  0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
  0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
  0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
  0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
  0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
  0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
  0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
  0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
  0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
  0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
  0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
  0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
  0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

inline uint32 crc32_adjust(uint32 crc32, uint8 input) {
  return ((crc32 >> 8) & 0x00ffffff) ^ crc32_table[(crc32 ^ input) & 0xff];
}

#endif
