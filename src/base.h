#include <time.h>
#include "lib/libbase.h"

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
