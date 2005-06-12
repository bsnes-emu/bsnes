/*
  libbase : version 0.01 ~byuu
*/

#ifndef __LIBBASE
#define __LIBBASE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  ulong;

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
