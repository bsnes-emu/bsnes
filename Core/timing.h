#ifndef timing_h
#define timing_h
#include "gb.h"

void GB_advance_cycles(GB_gameboy_t *gb, uint8_t cycles);
void GB_timers_run(GB_gameboy_t *gb);
void GB_rtc_run(GB_gameboy_t *gb);
#endif /* timing_h */
