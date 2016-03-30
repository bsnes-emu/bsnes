#ifndef memory_h
#define memory_h
#include "gb.h"

unsigned char read_memory(GB_gameboy_t *gb, unsigned short addr);
void write_memory(GB_gameboy_t *gb, unsigned short addr, unsigned char value);
void hdma_run(GB_gameboy_t *gb);

#endif /* memory_h */
