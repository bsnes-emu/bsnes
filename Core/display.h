#ifndef display_h
#define display_h

#include "gb.h"
void display_run(GB_gameboy_t *gb);
void palette_changed(GB_gameboy_t *gb, bool background_palette, unsigned char index);
#endif /* display_h */
