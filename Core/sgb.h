#ifndef sgb_h
#define sgb_h
#include "gb.h"

#ifdef GB_INTERNAL
void GB_sgb_write(GB_gameboy_t *gb, uint8_t value);
void GB_sgb_render(GB_gameboy_t *gb);
#endif

#endif
