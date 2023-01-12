#ifndef sm83_cpu_h
#define sm83_cpu_h
#include "defs.h"
#include <stdint.h>

void GB_cpu_disassemble(GB_gameboy_t *gb, uint16_t pc, uint16_t count);
#ifdef GB_INTERNAL
internal void GB_cpu_run(GB_gameboy_t *gb);
#endif

#endif
