#include "sgb.h"

void GB_sgb_write(GB_gameboy_t *gb, uint8_t value)
{
    if (!GB_is_sgb(gb)) return;
    switch ((value >> 4) & 3 ) {
        case 3:
            gb->sgb_ready_for_pulse = true;
            break;
            
        case 2: // Zero
            if (!gb->sgb_ready_for_pulse || !gb->sgb_ready_for_write) return;
            if (gb->sgb_command_write_index >= sizeof(gb->sgb_command) * 8) {
                GB_log(gb, "Got SGB command: ");
                for (unsigned i = 0; i < 16; i++) {
                    GB_log(gb, "%02x ", gb->sgb_command[i]);
                }
                GB_log(gb, "\n");
                gb->sgb_ready_for_pulse = false;
                gb->sgb_ready_for_write = false;
            }
            else {
                gb->sgb_command_write_index++;
                gb->sgb_ready_for_pulse = false;
            }
            break;
        case 1: // One
            if (!gb->sgb_ready_for_pulse || !gb->sgb_ready_for_write) return;
            if (gb->sgb_command_write_index >= sizeof(gb->sgb_command) * 8) {
                GB_log(gb, "Corrupt SGB command.\n");
                gb->sgb_ready_for_pulse = false;
                gb->sgb_ready_for_write = false;
            }
            else {
                gb->sgb_command[gb->sgb_command_write_index / 8] |= 1 << (gb->sgb_command_write_index & 7);
                gb->sgb_command_write_index++;
                gb->sgb_ready_for_pulse = false;
            }
            break;
        
        case 0:
            gb->sgb_ready_for_pulse = false;
            gb->sgb_ready_for_write = true;
            gb->sgb_command_write_index = 0;
            memset(gb->sgb_command, 0, sizeof(gb->sgb_command));
            break;
            
        default:
            break;
    }
}
