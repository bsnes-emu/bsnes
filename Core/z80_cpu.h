#ifndef z80_cpu_h
#define z80_cpu_h
#include "gb.h"
void cpu_disassemble(GB_gameboy_t *gb, unsigned short pc, unsigned short count);
void cpu_run(GB_gameboy_t *gb);

#endif /* z80_cpu_h */
