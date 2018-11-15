#ifndef sgb_h
#define sgb_h
#include "gb_struct_def.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef GB_INTERNAL
typedef struct {
    uint8_t command[16 * 7];
    uint16_t command_write_index;
    bool ready_for_pulse;
    bool ready_for_write;
    bool ready_for_stop;
    bool disable_commands;
    
    /* Screen buffer */
    uint8_t screen_buffer[160 * 144];
    /* Multiplayer Input */
    uint8_t player_count, current_player;
} GB_sgb_t;

void GB_sgb_write(GB_gameboy_t *gb, uint8_t value);
void GB_sgb_render(GB_gameboy_t *gb);

#endif

#endif
