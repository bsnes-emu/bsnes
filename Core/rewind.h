#ifndef rewind_h
#define rewind_h

#include <stdbool.h>
#include "defs.h"

#ifdef GB_INTERNAL
internal void GB_rewind_push(GB_gameboy_t *gb);
#endif
bool GB_rewind_pop(GB_gameboy_t *gb);
void GB_set_rewind_length(GB_gameboy_t *gb, double seconds);
void GB_rewind_reset(GB_gameboy_t *gb);

#endif
