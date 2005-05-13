//platform-independant includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//typecasts
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  ulong;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;

typedef signed char    int8;
typedef signed short   int16;
typedef signed long    int32;

//structs
typedef struct {
uint8 *data;
uint32 size;
}lfile;

//platform-specific global functions
void *memalloc(uint32 size, char *name = 0, ...);
void memfree(void *mem, char *name = 0, ...);
void alert(char *s, ...);
void dprintf(char *s, ...);

//various class interfaces
#include "interface.h"
