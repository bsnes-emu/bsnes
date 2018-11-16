#include "gb.h"

enum {
    MLT_REQ = 0x11,
    MASK_EN = 0x17,
};

typedef enum {
    MASK_DISABLED,
    MASK_FREEZE,
    MASK_COLOR_3,
    MASK_COLOR_0,
} mask_mode_t;

#define SGB_PACKET_SIZE 16

static void command_ready(GB_gameboy_t *gb)
{
    /* SGB header commands are used to send the contents of the header to the SNES CPU.
       A header command looks like this:
       Command ID: 0b1111xxx1, where xxx is the packet index. (e.g. F1 for [0x104, 0x112), F3 for [0x112, 0x120))
       Checksum: Simple one byte sum for the following content bytes
       0xE content bytes. The last command, FB, is padded with zeros, so information past the header is not sent. */
    
    
    if ((gb->sgb->command[0] & 0xF1) == 0xF1) {
        uint8_t checksum = 0;
        for (unsigned i = 2; i < 0x10; i++) {
            checksum += gb->sgb->command[i];
        }
        if (checksum != gb->sgb->command[1]) {
            GB_log(gb, "Failed checksum for SGB header command, disabling SGB features\n");
            gb->sgb->disable_commands = true;
            return;
        }
        if (gb->sgb->command[0] == 0xf9) {
            if (gb->sgb->command[0xc] != 3) { // SGB Flag
                GB_log(gb, "SGB flag is not 0x03, disabling SGB features\n");
                gb->sgb->disable_commands = true;
            }
        }
        else if (gb->sgb->command[0] == 0xfb) {
            if (gb->sgb->command[0x3] != 0x33) { // Old licensee code
                GB_log(gb, "Old licensee code is not 0x33, disabling SGB features\n");
                gb->sgb->disable_commands = true;
            }
        }
        return;
    }
    

    switch (gb->sgb->command[0] >> 3) {
        case MLT_REQ:
            gb->sgb->player_count = (uint8_t[]){1, 2, 1, 4}[gb->sgb->command[1] & 3];
            gb->sgb->current_player = gb->sgb->player_count - 1;
            break;
        case MASK_EN:
            gb->sgb->mask_mode = gb->sgb->command[1] & 3;
            break;
        default:
            GB_log(gb, "Unimplemented SGB command %x: ", gb->sgb->command[0] >> 3);
            for (unsigned i = 0; i < gb->sgb->command_write_index / 8; i++) {
                GB_log(gb, "%02x ", gb->sgb->command[i]);
            }
            GB_log(gb, "\n");
            ;

    }
}

void GB_sgb_write(GB_gameboy_t *gb, uint8_t value)
{
    if (!GB_is_sgb(gb)) return;
    if (gb->sgb->disable_commands) return;
    if (gb->sgb->command_write_index >= sizeof(gb->sgb->command) * 8) return;
    
    uint16_t command_size = (gb->sgb->command[0] & 7 ?: 1) * SGB_PACKET_SIZE * 8;
    if ((gb->sgb->command[0] & 0xF1) == 0xF1) {
        command_size = SGB_PACKET_SIZE * 8;
    }
    
    switch ((value >> 4) & 3) {
        case 3:
            gb->sgb->ready_for_pulse = true;
            break;
            
        case 2: // Zero
            if (!gb->sgb->ready_for_pulse || !gb->sgb->ready_for_write) return;
            if (gb->sgb->ready_for_stop) {
                if (gb->sgb->command_write_index == command_size) {
                    command_ready(gb);
                    gb->sgb->command_write_index = 0;
                    memset(gb->sgb->command, 0, sizeof(gb->sgb->command));
                }
                gb->sgb->ready_for_pulse = false;
                gb->sgb->ready_for_write = false;
                gb->sgb->ready_for_stop = false;
            }
            else {
                gb->sgb->command_write_index++;
                gb->sgb->ready_for_pulse = false;
                if (((gb->sgb->command_write_index) & (SGB_PACKET_SIZE * 8 - 1)) == 0) {
                    gb->sgb->ready_for_stop = true;
                }
            }
            break;
        case 1: // One
            if (!gb->sgb->ready_for_pulse || !gb->sgb->ready_for_write) return;
            if (gb->sgb->ready_for_stop) {
                GB_log(gb, "Corrupt SGB command.\n");
                gb->sgb->ready_for_pulse = false;
                gb->sgb->ready_for_write = false;
                gb->sgb->command_write_index = 0;
                memset(gb->sgb->command, 0, sizeof(gb->sgb->command));
            }
            else {
                gb->sgb->command[gb->sgb->command_write_index / 8] |= 1 << (gb->sgb->command_write_index & 7);
                gb->sgb->command_write_index++;
                gb->sgb->ready_for_pulse = false;
                if (((gb->sgb->command_write_index) & (SGB_PACKET_SIZE * 8 - 1)) == 0) {
                    gb->sgb->ready_for_stop = true;
                }
            }
            break;
        
        case 0:
            if (!gb->sgb->ready_for_pulse) return;
            gb->sgb->ready_for_pulse = false;
            gb->sgb->ready_for_write = true;
            gb->sgb->ready_for_pulse = false;
            if (gb->sgb->player_count > 1 && (value & 0x30) != (gb->io_registers[GB_IO_JOYP] & 0x30)) {
                gb->sgb->current_player++;
                gb->sgb->current_player &= gb->sgb->player_count - 1;
            }
            break;
            
        default:
            break;
    }
}

void GB_sgb_render(GB_gameboy_t *gb)
{
    if (!gb->screen || !gb->rgb_encode_callback) return;
    
    uint32_t border = gb->rgb_encode_callback(gb, 0x9c, 0x9c, 0xa5);
    uint32_t colors[] = {
        gb->rgb_encode_callback(gb, 0xf7, 0xe7, 0xc6),
        gb->rgb_encode_callback(gb, 0xd6, 0x8e, 0x49),
        gb->rgb_encode_callback(gb, 0xa6, 0x37, 0x25),
        gb->rgb_encode_callback(gb, 0x33, 0x1e, 0x50)
    };
    
    for (unsigned i = 0; i < 256 * 224; i++) {
        gb->screen[i] = border;
    }
    
    switch ((mask_mode_t) gb->sgb->mask_mode) {
        case MASK_DISABLED:
            memcpy(gb->sgb->effective_screen_buffer,
                   gb->sgb->screen_buffer,
                   sizeof(gb->sgb->effective_screen_buffer));
            break;
        case MASK_FREEZE:
            break;
        
        case MASK_COLOR_3:
            memset(gb->sgb->effective_screen_buffer,
                   3,
                   sizeof(gb->sgb->effective_screen_buffer));
            break;
        case MASK_COLOR_0:
            memset(gb->sgb->effective_screen_buffer,
                   0,
                   sizeof(gb->sgb->effective_screen_buffer));
    }
    
    uint32_t *output = &gb->screen[48 + 39 * 256];
    uint8_t *input = gb->sgb->effective_screen_buffer;
    for (unsigned y = 0; y < 144; y++) {
        for (unsigned x = 0; x < 160; x++) {
            *(output++) = colors[*(input++) & 3];
        }
        output += 256 - 160;
    }
}
