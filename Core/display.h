#ifndef display_h
#define display_h

#include "gb.h"
void GB_display_run(GB_gameboy_t *gb);
void GB_palette_changed(GB_gameboy_t *gb, bool background_palette, uint8_t index);
#endif /* display_h */
