#ifndef timing_h
#define timing_h
#include "gb.h"

void advance_cycles(GB_gameboy_t *gb, unsigned char cycles);
void timers_run(GB_gameboy_t *gb);
#endif /* timing_h */
