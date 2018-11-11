#include "sgb.h"

enum {
    MLT_REQ = 0x11,
};

static void command_ready(GB_gameboy_t *gb)
{
    /* SGB header commands are used to send the contents of the header to the SNES CPU.
       A header command looks like this:
       Command ID: 0b1111xxx1, where xxx is the packet index. (e.g. F1 for [0x104, 0x112), F3 for [0x112, 0x120))
       Checksum: Simple one byte sum for the following content bytes
       0xE content bytes. The last command, FB, is padded with zeros, so information past the header is not sent. */
    
    if (gb->sgb_command[0] >= 0xF0) {
        uint8_t checksum = 0;
        for (unsigned i = 2; i < 0x10; i++) {
            checksum += gb->sgb_command[i];
        }
        if (checksum != gb->sgb_command[1]) {
            GB_log(gb, "Failed checksum for SGB header command, disabling SGB features\n");
            gb->sgb_disable_commands = true;
            return;
        }
    }
    if (gb->sgb_command[0] == 0xf9) {
        if (gb->sgb_command[0xc] != 3) { // SGB Flag
            GB_log(gb, "SGB flag is not 0x03, disabling SGB features\n");
            gb->sgb_disable_commands = true;
        }
    }
    else if (gb->sgb_command[0] == 0xfb) {
        if (gb->sgb_command[0x3] != 0x33) { // Old licensee code
            GB_log(gb, "Old licensee code is not 0x33, disabling SGB features\n");
            gb->sgb_disable_commands = true;
        }
    }
    
    switch (gb->sgb_command[0] >> 3) {
        case MLT_REQ:
            gb->sgb_player_count = (uint8_t[]){1, 2, 1, 4}[gb->sgb_command[1] & 3];
            gb->sgb_current_player = gb->sgb_player_count - 1;
            break;
    }
}

void GB_sgb_write(GB_gameboy_t *gb, uint8_t value)
{
    if (!GB_is_sgb(gb)) return;
    if (gb->sgb_disable_commands) return;
    
    switch ((value >> 4) & 3) {
        case 3:
            gb->sgb_ready_for_pulse = true;
            break;
            
        case 2: // Zero
            if (!gb->sgb_ready_for_pulse || !gb->sgb_ready_for_write) return;
            if (gb->sgb_command_write_index >= sizeof(gb->sgb_command) * 8) {
                command_ready(gb);
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
            if (gb->sgb_player_count > 1 && (value & 0x30) != (gb->io_registers[GB_IO_JOYP] & 0x30)) {
                gb->sgb_current_player++;
                gb->sgb_current_player &= gb->sgb_player_count - 1;
            }
            break;
            
        default:
            break;
    }
}
