#ifndef rumble_h
#define rumble_h

#include "defs.h"

typedef enum {
    GB_RUMBLE_DISABLED,
    GB_RUMBLE_CARTRIDGE_ONLY,
    GB_RUMBLE_ALL_GAMES
} GB_rumble_mode_t;

#ifdef GB_INTERNAL
internal void GB_handle_rumble(GB_gameboy_t *gb);
#endif
void GB_set_rumble_mode(GB_gameboy_t *gb, GB_rumble_mode_t mode);

#endif
