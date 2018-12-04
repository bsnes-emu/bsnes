#include "gb.h"
#include <assert.h>

void GB_update_joyp(GB_gameboy_t *gb)
{
    uint8_t key_selection = 0;
    uint8_t previous_state = 0;

    /* Todo: add delay to key selection */
    previous_state = gb->io_registers[GB_IO_JOYP] & 0xF;
    key_selection = (gb->io_registers[GB_IO_JOYP] >> 4) & 3;
    gb->io_registers[GB_IO_JOYP] &= 0xF0;
    uint8_t current_player = gb->sgb? gb->sgb->current_player : 0;
    switch (key_selection) {
        case 3:
            if (gb->sgb && gb->sgb->player_count > 1) {
                gb->io_registers[GB_IO_JOYP] |= 0xF - current_player;
            }
            else {
                /* Nothing is wired, all up */
                gb->io_registers[GB_IO_JOYP] |= 0x0F;
            }
            break;

        case 2:
            /* Direction keys */
            for (uint8_t i = 0; i < 4; i++) {
                gb->io_registers[GB_IO_JOYP] |= (!gb->keys[current_player][i]) << i;
            }
            /* Forbid pressing two opposing keys, this breaks a lot of games; even if it's somewhat possible. */
            if (!(gb->io_registers[GB_IO_JOYP] & 1)) {
                gb->io_registers[GB_IO_JOYP] |= 2;
            }
            if (!(gb->io_registers[GB_IO_JOYP] & 4)) {
                gb->io_registers[GB_IO_JOYP] |= 8;
            }
            break;

        case 1:
            /* Other keys */
            for (uint8_t i = 0; i < 4; i++) {
                gb->io_registers[GB_IO_JOYP] |= (!gb->keys[current_player][i + 4]) << i;
            }
            break;

        case 0:
            for (uint8_t i = 0; i < 4; i++) {
                gb->io_registers[GB_IO_JOYP] |= (!(gb->keys[current_player][i] || gb->keys[current_player][i + 4])) << i;
            }
            break;

        default:
            break;
    }
    
    if (previous_state != (gb->io_registers[GB_IO_JOYP] & 0xF)) {
        /* The joypad interrupt DOES occur on CGB (Tested on CGB-CPU-06), unlike what some documents say. */
        gb->io_registers[GB_IO_IF] |= 0x10;
        gb->stopped = false;
    }
    
    gb->io_registers[GB_IO_JOYP] |= 0xC0;
}

void GB_set_key_state(GB_gameboy_t *gb, GB_key_t index, bool pressed)
{
    assert(index >= 0 && index < GB_KEY_MAX);
    gb->keys[0][index] = pressed;
}

void GB_set_key_state_for_player(GB_gameboy_t *gb, GB_key_t index, unsigned player, bool pressed)
{
    assert(index >= 0 && index < GB_KEY_MAX);
    assert(player < 4);
    gb->keys[player][index] = pressed;
}
