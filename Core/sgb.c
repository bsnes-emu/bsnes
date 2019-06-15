#include "gb.h"
#include "random.h"
#include <math.h>

#define INTRO_ANIMATION_LENGTH 200

enum {
    PAL01    = 0x00,
    PAL23    = 0x01,
    PAL03    = 0x02,
    PAL12    = 0x03,
    ATTR_BLK = 0x04,
    ATTR_LIN = 0x05,
    ATTR_DIV = 0x06,
    PAL_SET  = 0x0A,
    PAL_TRN  = 0x0B,
    DATA_SND = 0x0F,
    MLT_REQ  = 0x11,
    CHR_TRN  = 0x13,
    PCT_TRN  = 0x14,
    ATTR_TRN = 0x15,
    ATTR_SET = 0x16,
    MASK_EN  = 0x17,
};

typedef enum {
    MASK_DISABLED,
    MASK_FREEZE,
    MASK_BLACK,
    MASK_COLOR_0,
} mask_mode_t;

typedef enum {
    TRANSFER_LOW_TILES,
    TRANSFER_HIGH_TILES,
    TRANSFER_BORDER_DATA,
    TRANSFER_PALETTES,
    TRANSFER_ATTRIBUTES,
} transfer_dest_t;

#define SGB_PACKET_SIZE 16
static inline void pal_command(GB_gameboy_t *gb, unsigned first, unsigned second)
{
    gb->sgb->effective_palettes[0] = gb->sgb->effective_palettes[4] =
        gb->sgb->effective_palettes[8] = gb->sgb->effective_palettes[12] =
        gb->sgb->command[1] | (gb->sgb->command[2] << 8);
    
    for (unsigned i = 0; i < 3; i++) {
        gb->sgb->effective_palettes[first * 4 + i + 1] = gb->sgb->command[3 + i * 2] | (gb->sgb->command[4 + i * 2] << 8);
    }
    
    for (unsigned i = 0; i < 3; i++) {
        gb->sgb->effective_palettes[second * 4 + i + 1] = gb->sgb->command[9 + i * 2] | (gb->sgb->command[10 + i * 2] << 8);
    }
}

static inline void load_attribute_file(GB_gameboy_t *gb, unsigned file_index)
{
    if (file_index > 0x2C) return;
    uint8_t *output = gb->sgb->attribute_map;
    for (unsigned i = 0; i < 90; i++) {
        uint8_t byte = gb->sgb->attribute_files[file_index * 90 + i];
        for (unsigned j = 4; j--;) {
            *(output++) = byte >> 6;
            byte <<= 2;
        }
    }
}

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
                gb->sgb->disable_commands = true;
            }
        }
        else if (gb->sgb->command[0] == 0xfb) {
            if (gb->sgb->command[0x3] != 0x33) { // Old licensee code
                gb->sgb->disable_commands = true;
            }
        }
        return;
    }
    
    /* Ignore malformed commands (0 length)*/
    if ((gb->sgb->command[0] & 7) == 0) return;
    
    switch (gb->sgb->command[0] >> 3) {
        case PAL01:
            pal_command(gb, 0, 1);
            break;
        case PAL23:
            pal_command(gb, 2, 3);
            break;
        case PAL03:
            pal_command(gb, 0, 3);
            break;
        case PAL12:
            pal_command(gb, 1, 2);
            break;
        case ATTR_BLK: {
            struct {
                uint8_t count;
                struct {
                    uint8_t control;
                    uint8_t palettes;
                    uint8_t left, top, right, bottom;
                } data[];
            } *command = (void *)(gb->sgb->command + 1);
            if (command->count > 0x12) return;
            
            for (unsigned i = 0; i < command->count; i++) {
                bool inside  = command->data[i].control & 1;
                bool middle  = command->data[i].control & 2;
                bool outside = command->data[i].control & 4;
                uint8_t inside_palette = command->data[i].palettes & 0x3;
                uint8_t middle_palette = (command->data[i].palettes >> 2) & 0x3;
                uint8_t outside_palette = (command->data[i].palettes >> 4) & 0x3;
                
                if (inside && !middle && !outside) {
                    middle = true;
                    middle_palette = inside_palette;
                }
                else if (outside && !middle && !inside) {
                    middle = true;
                    middle_palette = outside_palette;
                }
                
                command->data[i].left &= 0x1F;
                command->data[i].top &= 0x1F;
                command->data[i].right &= 0x1F;
                command->data[i].bottom &= 0x1F;
                
                for (unsigned y = 0; y < 18; y++) {
                    for (unsigned x = 0; x < 20; x++) {
                        if (x < command->data[i].left || x > command->data[i].right ||
                            y < command->data[i].top  || y > command->data[i].bottom)  {
                            if (outside) {
                                gb->sgb->attribute_map[x + 20 * y] = outside_palette;
                            }
                        }
                        else if (x > command->data[i].left && x < command->data[i].right &&
                                 y > command->data[i].top  && y < command->data[i].bottom)  {
                            if (inside) {
                                gb->sgb->attribute_map[x + 20 * y] = inside_palette;
                            }
                        }
                        else if(middle) {
                            gb->sgb->attribute_map[x + 20 * y] = middle_palette;
                        }
                    }
                }
            }
            break;
        }
        case ATTR_LIN: {
            struct {
                uint8_t count;
                uint8_t data[];
            } *command = (void *)(gb->sgb->command + 1);
            if (command->count > sizeof(gb->sgb->command) - 2) return;
            
            for (unsigned i = 0; i < command->count; i++) {
                bool horizontal = command->data[i] & 0x80;
                uint8_t palette = (command->data[i] >> 5) & 0x3;
                uint8_t line = (command->data[i]) & 0x1F;
                
                if (horizontal) {
                    if (line > 18) continue;
                    for (unsigned x = 0; x < 20; x++) {
                        gb->sgb->attribute_map[x + 20 * line] = palette;
                    }
                }
                else {
                    if (line > 20) continue;
                    for (unsigned y = 0; y < 18; y++) {
                        gb->sgb->attribute_map[line + 20 * y] = palette;
                    }
                }
            }
            break;
        }
        case ATTR_DIV: {
            uint8_t high_palette = gb->sgb->command[1] & 3;
            uint8_t low_palette = (gb->sgb->command[1] >> 2) & 3;
            uint8_t middle_palette = (gb->sgb->command[1] >> 4) & 3;
            bool horizontal = gb->sgb->command[1] & 0x40;
            uint8_t line = gb->sgb->command[2] & 0x1F;
            
            for (unsigned y = 0; y < 18; y++) {
                for (unsigned x = 0; x < 20; x++) {
                    if ((horizontal? y : x) < line) {
                        gb->sgb->attribute_map[x + 20 * y] = low_palette;
                    }
                    else if ((horizontal? y : x) == line) {
                        gb->sgb->attribute_map[x + 20 * y] = middle_palette;
                    }
                    else {
                        gb->sgb->attribute_map[x + 20 * y] = high_palette;
                    }
                }
            }

            break;
        }
        case PAL_SET:
            memcpy(&gb->sgb->effective_palettes[0],
                   &gb->sgb->ram_palettes[4 * (gb->sgb->command[1] + (gb->sgb->command[2] & 1) * 0x100)],
                   8);
            memcpy(&gb->sgb->effective_palettes[4],
                   &gb->sgb->ram_palettes[4 * (gb->sgb->command[3] + (gb->sgb->command[4] & 1) * 0x100)],
                   8);
            memcpy(&gb->sgb->effective_palettes[8],
                   &gb->sgb->ram_palettes[4 * (gb->sgb->command[5] + (gb->sgb->command[6] & 1) * 0x100)],
                   8);
            memcpy(&gb->sgb->effective_palettes[12],
                   &gb->sgb->ram_palettes[4 * (gb->sgb->command[7] + (gb->sgb->command[8] & 1) * 0x100)],
                   8);
            
            gb->sgb->effective_palettes[12] = gb->sgb->effective_palettes[8] =
            gb->sgb->effective_palettes[4] = gb->sgb->effective_palettes[0];
            
            if (gb->sgb->command[9] & 0x80) {
                load_attribute_file(gb, gb->sgb->command[9] & 0x3F);
            }
            
            if (gb->sgb->command[9] & 0x40) {
                gb->sgb->mask_mode = MASK_DISABLED;
            }
            break;
        case PAL_TRN:
            gb->sgb->vram_transfer_countdown = 2;
            gb->sgb->transfer_dest = TRANSFER_PALETTES;
            break;
        case DATA_SND:
            // Not supported, but used by almost all SGB games for hot patching, so let's mute the warning for this
            break;
        case MLT_REQ:
            gb->sgb->player_count = (uint8_t[]){1, 2, 1, 4}[gb->sgb->command[1] & 3];
            gb->sgb->current_player = gb->sgb->player_count - 1;
            break;
        case CHR_TRN:
            gb->sgb->vram_transfer_countdown = 2;
            gb->sgb->transfer_dest = (gb->sgb->command[1] & 1)? TRANSFER_HIGH_TILES : TRANSFER_LOW_TILES;
            break;
        case PCT_TRN:
            gb->sgb->vram_transfer_countdown = 2;
            gb->sgb->transfer_dest = TRANSFER_BORDER_DATA;
            break;
        case ATTR_TRN:
            gb->sgb->vram_transfer_countdown = 2;
            gb->sgb->transfer_dest = TRANSFER_ATTRIBUTES;
            break;
        case ATTR_SET:
            load_attribute_file(gb, gb->sgb->command[0] & 0x3F);
            
            if (gb->sgb->command[0] & 0x40) {
                gb->sgb->mask_mode = MASK_DISABLED;
            }
            break;
        case MASK_EN:
            gb->sgb->mask_mode = gb->sgb->command[1] & 3;
            break;
        default:
            if ((gb->sgb->command[0] >> 3) == 8 &&
                (gb->sgb->command[1] & ~0x80) == 0  &&
                (gb->sgb->command[2] & ~0x80) == 0) {
                /* Mute/dummy sound commands, ignore this command as it's used by many games at startup */
                break;
            }
            GB_log(gb, "Unimplemented SGB command %x: ", gb->sgb->command[0] >> 3);
            for (unsigned i = 0; i < gb->sgb->command_write_index / 8; i++) {
                GB_log(gb, "%02x ", gb->sgb->command[i]);
            }
            GB_log(gb, "\n");
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
            /* TODO: This is the logic used by BGB which *should* work for most/all games, but a proper test ROM is needed */
            if (gb->sgb->player_count > 1 && (gb->io_registers[GB_IO_JOYP] & 0x30) == 0x10) {
                gb->sgb->current_player++;
                gb->sgb->current_player &= gb->sgb->player_count - 1;
            }
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
            gb->sgb->ready_for_write = true;
            gb->sgb->ready_for_pulse = false;
            if (((gb->sgb->command_write_index) & (SGB_PACKET_SIZE * 8 - 1)) != 0 ||
                gb->sgb->command_write_index == 0 ||
                gb->sgb->ready_for_stop) {
                gb->sgb->command_write_index = 0;
                memset(gb->sgb->command, 0, sizeof(gb->sgb->command));
                gb->sgb->ready_for_stop = false;
            }
            break;
            
        default:
            break;
    }
}

static inline uint8_t scale_channel(uint8_t x)
{
    return (x << 3) | (x >> 2);
}

static uint32_t convert_rgb15(GB_gameboy_t *gb, uint16_t color)
{
    uint8_t r = (color) & 0x1F;
    uint8_t g = (color >> 5) & 0x1F;
    uint8_t b = (color >> 10) & 0x1F;
    
    r = scale_channel(r);
    g = scale_channel(g);
    b = scale_channel(b);
    
    return gb->rgb_encode_callback(gb, r, g, b);
}

static uint32_t convert_rgb15_with_fade(GB_gameboy_t *gb, uint16_t color, uint8_t fade)
{
    uint8_t r = ((color) & 0x1F) - fade;
    uint8_t g = ((color >> 5) & 0x1F) - fade;
    uint8_t b = ((color >> 10) & 0x1F) - fade;
    
    if (r >= 0x20) r = 0;
    if (g >= 0x20) g = 0;
    if (b >= 0x20) b = 0;
    
    r = scale_channel(r);
    g = scale_channel(g);
    b = scale_channel(b);
    
    return gb->rgb_encode_callback(gb, r, g, b);
}

#include <stdio.h>
static void render_boot_animation (GB_gameboy_t *gb)
{
#include "sgb_animation_logo.inc"
    uint32_t *output = &gb->screen[48 + 40 * 256];
    uint8_t *input = animation_logo;
    unsigned fade_blue = 0;
    unsigned fade_red = 0;
    if (gb->sgb->intro_animation < 80 - 32) {
        fade_blue = 32;
    }
    else if (gb->sgb->intro_animation < 80) {
        fade_blue = 80 - gb->sgb->intro_animation;
    }
    else if (gb->sgb->intro_animation > INTRO_ANIMATION_LENGTH - 32) {
        fade_red = fade_blue = gb->sgb->intro_animation - INTRO_ANIMATION_LENGTH + 32;
    }
    uint32_t colors[] = {
        convert_rgb15(gb, 0),
        convert_rgb15_with_fade(gb, 0x14A5, fade_blue),
        convert_rgb15_with_fade(gb, 0x54E0, fade_blue),
        convert_rgb15_with_fade(gb, 0x0019, fade_red),
        convert_rgb15(gb, 0x0011),
        convert_rgb15(gb, 0x0009),
    };
    unsigned y_min = (144 - animation_logo_height) / 2;
    unsigned y_max = y_min + animation_logo_height;
    for (unsigned y = 0; y < 144; y++) {
        for (unsigned x = 0; x < 160; x++) {
            if (y < y_min || y >= y_max) {
                *(output++) = colors[0];
            }
            else {
                uint8_t color = *input;
                if (color >= 3) {
                    if (color == gb->sgb->intro_animation / 2 - 3) {
                        color = 5;
                    }
                    else if (color == gb->sgb->intro_animation / 2 - 4) {
                        color = 4;
                    }
                    else if (color < gb->sgb->intro_animation / 2 - 4) {
                        color = 3;
                    }
                    else {
                        color = 0;
                    }
                }
                *(output++) = colors[color];
                input++;
            }
        }
        output += 256 - 160;
    }
}

void GB_sgb_render(GB_gameboy_t *gb)
{
    if (gb->sgb->intro_animation < INTRO_ANIMATION_LENGTH) gb->sgb->intro_animation++;

    if (!gb->screen || !gb->rgb_encode_callback) return;
    
    if (gb->sgb->mask_mode != MASK_FREEZE) {
        memcpy(gb->sgb->effective_screen_buffer,
               gb->sgb->screen_buffer,
               sizeof(gb->sgb->effective_screen_buffer));
    }
    
    if (gb->sgb->vram_transfer_countdown) {
        if (--gb->sgb->vram_transfer_countdown == 0) {
            if (gb->sgb->transfer_dest == TRANSFER_LOW_TILES || gb->sgb->transfer_dest == TRANSFER_HIGH_TILES) {
                uint8_t *base = &gb->sgb->pending_border.tiles[gb->sgb->transfer_dest == TRANSFER_HIGH_TILES ? 0x80 * 8 * 8 : 0];
                for (unsigned tile = 0; tile < 0x80; tile++) {
                    unsigned tile_x = (tile % 10) * 16;
                    unsigned tile_y = (tile / 10) * 8;
                    for (unsigned y = 0; y < 0x8; y++) {
                        for (unsigned x = 0; x < 0x8; x++) {
                            base[tile * 8 * 8 + y * 8 + x] = gb->sgb->screen_buffer[(tile_x + x) + (tile_y + y) * 160] +
                                                             gb->sgb->screen_buffer[(tile_x + x + 8) + (tile_y + y) * 160] * 4;
                        }
                    }
                }
                
            }
            else {
                unsigned size = 0;
                uint16_t *data = NULL;
                
                switch (gb->sgb->transfer_dest) {
                    case TRANSFER_PALETTES:
                        size = 0x100;
                        data = gb->sgb->ram_palettes;
                        break;
                    case TRANSFER_BORDER_DATA:
                        size = 0x88;
                        data = gb->sgb->pending_border.raw_data;
                        break;
                    case TRANSFER_ATTRIBUTES:
                        size = 0xFE;
                        data = (uint16_t *)gb->sgb->attribute_files;
                        break;
                    default:
                        return; // Corrupt state?
                }
                
                for (unsigned tile = 0; tile < size; tile++) {
                    unsigned tile_x = (tile % 20) * 8;
                    unsigned tile_y = (tile / 20) * 8;
                    for (unsigned y = 0; y < 0x8; y++) {
                        static const uint16_t pixel_to_bits[4] = {0x0000, 0x0080, 0x8000, 0x8080};
                        *data = 0;
                        for (unsigned x = 0; x < 8; x++) {
                            *data |= pixel_to_bits[gb->sgb->screen_buffer[(tile_x + x) + (tile_y + y) * 160] & 3] >> x;
                        }
#ifdef GB_BIG_ENDIAN
                        if (gb->sgb->transfer_dest == TRANSFER_ATTRIBUTES) {
                            *data = __builtin_bswap16(*data);
                        }
#endif
                        data++;
                    }
                }
                if (gb->sgb->transfer_dest == TRANSFER_BORDER_DATA) {
                    gb->sgb->border_animation = 64;
                }
            }
        }
    }
    
    uint32_t colors[4 * 4];
    for (unsigned i = 0; i < 4 * 4; i++) {
        colors[i] = convert_rgb15(gb, gb->sgb->effective_palettes[i]);
    }
    
    if (gb->sgb->intro_animation < INTRO_ANIMATION_LENGTH) {
        render_boot_animation(gb);
    }
    else {
        uint32_t *output = &gb->screen[48 + 40 * 256];
        uint8_t *input = gb->sgb->effective_screen_buffer;
        switch ((mask_mode_t) gb->sgb->mask_mode) {
            case MASK_DISABLED:
            case MASK_FREEZE: {
                for (unsigned y = 0; y < 144; y++) {
                    for (unsigned x = 0; x < 160; x++) {
                        uint8_t palette = gb->sgb->attribute_map[x / 8 + y / 8 * 20] & 3;
                        *(output++) = colors[(*(input++) & 3) + palette * 4];
                    }
                    output += 256 - 160;
                }
                break;
            }
            case MASK_BLACK:
            {
                uint32_t black = convert_rgb15(gb, 0);
                for (unsigned y = 0; y < 144; y++) {
                    for (unsigned x = 0; x < 160; x++) {
                        *(output++) = black;
                    }
                    output += 256 - 160;
                }
                break;
            }
            case MASK_COLOR_0:
            {
                for (unsigned y = 0; y < 144; y++) {
                    for (unsigned x = 0; x < 160; x++) {
                        *(output++) = colors[0];
                    }
                    output += 256 - 160;
                }
                break;
            }
        }
    }
    
    uint32_t border_colors[16 * 4];
    if (gb->sgb->border_animation == 0 || gb->sgb->intro_animation < INTRO_ANIMATION_LENGTH) {
        for (unsigned i = 0; i < 16 * 4; i++) {
            border_colors[i] = convert_rgb15(gb, gb->sgb->border.palette[i]);
        }
    }
    else if (gb->sgb->border_animation > 32) {
        gb->sgb->border_animation--;
        for (unsigned i = 0; i < 16 * 4; i++) {
            border_colors[i] = convert_rgb15_with_fade(gb, gb->sgb->border.palette[i], 64 - gb->sgb->border_animation);
        }
    }
    else {
        gb->sgb->border_animation--;
        for (unsigned i = 0; i < 16 * 4; i++) {
            border_colors[i] = convert_rgb15_with_fade(gb, gb->sgb->border.palette[i], gb->sgb->border_animation);
        }
    }
    
    
    if (gb->sgb->border_animation == 32) {
        memcpy(&gb->sgb->border, &gb->sgb->pending_border, sizeof(gb->sgb->border));
    }
    
    for (unsigned tile_y = 0; tile_y < 28; tile_y++) {
        for (unsigned tile_x = 0; tile_x < 32; tile_x++) {
            bool gb_area = false;
            if (tile_x >= 6 && tile_x < 26 && tile_y >= 5 && tile_y < 23) {
                gb_area = true;
            }
            uint16_t tile = gb->sgb->border.map[tile_x + tile_y * 32];
            uint8_t flip_x = (tile & 0x4000)? 0x7 : 0;
            uint8_t flip_y = (tile & 0x8000)? 0x7 : 0;
            uint8_t palette = (tile >> 10) & 3;
            for (unsigned y = 0; y < 8; y++) {
                for (unsigned x = 0; x < 8; x++) {
                    uint8_t color = gb->sgb->border.tiles[(tile & 0xFF) * 64 + (x ^ flip_x) + (y ^ flip_y) * 8] & 0xF;
                    if (color == 0) {
                        if (gb_area) continue;
                        gb->screen[tile_x * 8 + x + (tile_y * 8 + y) * 0x100] = colors[0];
                    }
                    else {
                        gb->screen[tile_x * 8 + x + (tile_y * 8 + y) * 0x100] = border_colors[color + palette * 16];
                    }
                }
            }
        }
    }
}

void GB_sgb_load_default_data(GB_gameboy_t *gb)
{
    
#include "sgb_border.inc"
    
    memcpy(gb->sgb->border.map, tilemap, sizeof(tilemap));
    memcpy(gb->sgb->border.palette, palette, sizeof(palette));
    
    /* Expend tileset */
    for (unsigned tile = 0; tile < sizeof(tiles) / 32; tile++) {
        for (unsigned y = 0; y < 8; y++) {
            for (unsigned x = 0; x < 8; x++) {
                gb->sgb->border.tiles[tile * 8 * 8 + y * 8 + x] =
                    (tiles[tile * 32 + y * 2 +  0] & (1 << (7 ^ x)) ? 1 : 0) |
                    (tiles[tile * 32 + y * 2 +  1] & (1 << (7 ^ x)) ? 2 : 0) |
                    (tiles[tile * 32 + y * 2 + 16] & (1 << (7 ^ x)) ? 4 : 0) |
                    (tiles[tile * 32 + y * 2 + 17] & (1 << (7 ^ x)) ? 8 : 0);
            }
        }
    }
    
    if (gb->model != GB_MODEL_SGB2) {
        /* Delete the "2" */
        gb->sgb->border.map[25 * 32 + 25] = gb->sgb->border.map[25 * 32 + 26] =
        gb->sgb->border.map[26 * 32 + 25] = gb->sgb->border.map[26 * 32 + 26] =
        gb->sgb->border.map[27 * 32 + 25] = gb->sgb->border.map[27 * 32 + 26] =
        gb->sgb->border.map[0];
        
        /* Re-center */
        memmove(&gb->sgb->border.map[25 * 32 + 1], &gb->sgb->border.map[25 * 32], (32 * 3 - 1) * sizeof(gb->sgb->border.map[0]));
    }
    gb->sgb->effective_palettes[0] = 0x639E;
    gb->sgb->effective_palettes[1] = 0x263A;
    gb->sgb->effective_palettes[2] = 0x10D4;
    gb->sgb->effective_palettes[3] = 0x2866;
}

static double fm_synth(double phase)
{
    return (sin(phase * M_PI * 2) +
           sin(phase * M_PI * 2 + sin(phase * M_PI * 2)) +
           sin(phase * M_PI * 2 + sin(phase * M_PI * 3)) +
           sin(phase * M_PI * 2 + sin(phase * M_PI * 4))) / 4;
}

static double fm_sweep(double phase)
{
    double ret = 0;
    for (unsigned i = 0; i < 8; i++) {
        ret += sin((phase * M_PI * 2 + sin(phase * M_PI * 8) / 4) * pow(1.25, i)) * (8 - i) / 36;
    }
    return ret;
}
static double random_double(void)
{
    return ((signed)(GB_random32() % 0x10001) - 0x8000) / (double) 0x8000;
}

bool GB_sgb_render_jingle(GB_gameboy_t *gb, GB_sample_t *dest, size_t count)
{
    const double frequencies[7] = {
        466.16, // Bb4
        587.33, // D5
        698.46, // F5
        830.61, // Ab5
        1046.50, // C6
        1244.51, // Eb6
        1567.98, // G6
    };
    
    if (gb->sgb->intro_animation < 0) {
        for (unsigned i = 0; i < count; i++) {
            dest->left = dest->right = 0;
            dest++;
        }
        return true;
    }
    
    if (gb->sgb->intro_animation >= INTRO_ANIMATION_LENGTH) return false;
    
    signed jingle_stage = (gb->sgb->intro_animation - 64) / 3;
    double sweep_cutoff_ratio = 2000.0 * pow(2, gb->sgb->intro_animation / 20.0) / gb->apu_output.sample_rate;
    double sweep_phase_shift = 1000.0 * pow(2, gb->sgb->intro_animation / 40.0) / gb->apu_output.sample_rate;
    if (sweep_cutoff_ratio > 1) {
        sweep_cutoff_ratio = 1;
    }
    
    for (unsigned i = 0; i < count; i++) {
        double sample = 0;
        for (signed f = 0; f < 7 && f < jingle_stage; f++) {
            sample += fm_synth(gb->sgb_intro_jingle_phases[f]) *
                      (0.75 * pow(0.5, jingle_stage - f) + 0.25) / 5.0;
            gb->sgb_intro_jingle_phases[f] += frequencies[f] / gb->apu_output.sample_rate;
        }
        if (gb->sgb->intro_animation > 100) {
            sample *= pow((INTRO_ANIMATION_LENGTH - gb->sgb->intro_animation) / (INTRO_ANIMATION_LENGTH - 100.0), 3);
        }
        
        if (gb->sgb->intro_animation < 120) {
            double next = fm_sweep(gb->sgb_intro_sweep_phase) * 0.3 + random_double() * 0.7;
            gb->sgb_intro_sweep_phase += sweep_phase_shift;

            gb->sgb_intro_sweep_previous_sample = next * (sweep_cutoff_ratio) +
                                                  gb->sgb_intro_sweep_previous_sample * (1 - sweep_cutoff_ratio);
            sample += gb->sgb_intro_sweep_previous_sample * pow((120 - gb->sgb->intro_animation) / 120.0, 2) * 0.8;
        }
        
        dest->left = dest->right = sample * 0x7000;
        dest++;
    }
    
    return true;
}

