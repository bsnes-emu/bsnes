/*
  libbase : version 0.03 ~byuu (08/20/05)
*/

#ifndef __LIBBASE
#define __LIBBASE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define zerofree(__n) if(__n) { free(__n); __n = 0; }

typedef unsigned int   uint;

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  ulong;

typedef unsigned char  bool8;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;

typedef signed char    int8;
typedef signed short   int16;
typedef signed long    int32;

#ifdef null
#undef null
#endif
#define null 0xffffffff

#endif
