#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "gb.h"
#include "display.h"
#ifdef _WIN32
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#endif

/*
 Each line is 456 cycles, approximately:
 Mode 2 - 80  cycles / OAM Transfer
 Mode 3 - 172 cycles / Rendering
 Mode 0 - 204 cycles / HBlank
 
 Mode 1 is VBlank
 
 Todo: Mode lengths are not constants, see http://blog.kevtris.org/blogfiles/Nitty%20Gritty%20Gameboy%20VRAM%20Timing.txt
 */

#define MODE2_LENGTH (80)
#define MODE3_LENGTH (172)
#define MODE0_LENGTH (204)
#define LINE_LENGTH (MODE2_LENGTH + MODE3_LENGTH + MODE0_LENGTH) // = 456
#define LINES (144)
#define WIDTH (160)
#define VIRTUAL_LINES (LCDC_PERIOD / LINE_LENGTH) // = 154

typedef struct __attribute__((packed)) {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    uint8_t flags;
} GB_sprite_t;

static uint32_t get_pixel(GB_gameboy_t *gb, uint8_t x, uint8_t y)
{
    /*
     Bit 7 - LCD Display Enable             (0=Off, 1=On)
     Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
     Bit 5 - Window Display Enable          (0=Off, 1=On)
     Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
     Bit 3 - BG Tile Map Display Select     (0=9800-9BFF, 1=9C00-9FFF)
     Bit 2 - OBJ (Sprite) Size              (0=8x8, 1=8x16)
     Bit 1 - OBJ (Sprite) Display Enable    (0=Off, 1=On)
     Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)
     */
    uint16_t map = 0x1800;
    uint8_t tile = 0;
    uint8_t attributes = 0;
    uint8_t sprite_palette = 0;
    uint16_t tile_address = 0;
    uint8_t background_pixel = 0, sprite_pixel = 0;
    GB_sprite_t *sprite = (GB_sprite_t *) &gb->oam;
    uint8_t sprites_in_line = 0;
    bool lcd_8_16_mode = (gb->io_registers[GB_IO_LCDC] & 4) != 0;
    bool sprites_enabled = (gb->io_registers[GB_IO_LCDC] & 2) != 0;
    uint8_t lowest_sprite_x = 0xFF;
    bool use_obp1 = false, priority = false;
    bool in_window = false;
    bool window_enabled = (gb->io_registers[GB_IO_LCDC] & 0x20);
    bool bg_enabled = true;
    bool bg_behind = false;
    if ((gb->io_registers[GB_IO_LCDC] & 0x1) == 0) {
        if (gb->cgb_mode) {
            bg_behind = true;
        }
        else if (gb->is_cgb) { /* CGB in DMG mode*/
            bg_enabled = window_enabled = false;
        }
        else {
            /* DMG */
            bg_enabled = false;
        }
    }
    if (gb->effective_window_enabled && window_enabled) { /* Window Enabled */
        if (y >= gb->effective_window_y && x + 7 >= gb->io_registers[GB_IO_WX]) {
            in_window = true;
        }
    }

    if (sprites_enabled) {
        // Loop all sprites
        for (uint8_t i = 40; i--; sprite++) {
            int sprite_y = sprite->y - 16;
            int sprite_x = sprite->x - 8;
            // Is sprite in our line?
            if (sprite_y <= y && sprite_y + (lcd_8_16_mode? 16:8) > y) {
                uint8_t tile_x, tile_y, current_sprite_pixel;
                uint16_t line_address;
                // Limit to 10 sprites in one scan line.
                if (++sprites_in_line == 11) break;
                // Does not overlap our pixel.
                if (sprite_x > x || sprite_x + 8 <= x) continue;
                tile_x = x - sprite_x;
                tile_y = y - sprite_y;
                if (sprite->flags & 0x20) tile_x = 7 - tile_x;
                if (sprite->flags & 0x40) tile_y = (lcd_8_16_mode? 15:7) - tile_y;
                line_address = (lcd_8_16_mode? sprite->tile & 0xFE : sprite->tile) * 0x10 + tile_y * 2;
                if (gb->cgb_mode && (sprite->flags & 0x8)) {
                    line_address += 0x2000;
                }
                current_sprite_pixel = (((gb->vram[line_address    ] >> ((~tile_x)&7)) & 1 ) |
                                        ((gb->vram[line_address + 1] >> ((~tile_x)&7)) & 1) << 1 );
                /* From Pandocs:
                     When sprites with different x coordinate values overlap, the one with the smaller x coordinate
                     (closer to the left) will have priority and appear above any others. This applies in Non CGB Mode
                     only. When sprites with the same x coordinate values overlap, they have priority according to table
                     ordering. (i.e. $FE00 - highest, $FE04 - next highest, etc.) In CGB Mode priorities are always
                     assigned like this.
                 */
                if (current_sprite_pixel != 0) {
                    if (!gb->cgb_mode && sprite->x >= lowest_sprite_x) {
                        break;
                    }
                    sprite_pixel = current_sprite_pixel;
                    lowest_sprite_x = sprite->x;
                    use_obp1 = (sprite->flags & 0x10) != 0;
                    sprite_palette = sprite->flags & 7;
                    priority = (sprite->flags & 0x80) != 0;
                    if (gb->cgb_mode) {
                        break;
                    }
                }
            }
        }
    }

    if (in_window) {
        x -= gb->io_registers[GB_IO_WX] - 7;
        y -= gb->effective_window_y;
    }
    else {
        x += gb->effective_scx;
        y += gb->io_registers[GB_IO_SCY];
    }
    if (gb->io_registers[GB_IO_LCDC] & 0x08 && !in_window) {
        map = 0x1C00;
    }
    else if (gb->io_registers[GB_IO_LCDC] & 0x40 && in_window) {
        map = 0x1C00;
    }
    tile = gb->vram[map + x/8 + y/8 * 32];
    if (gb->cgb_mode) {
        attributes = gb->vram[map + x/8 + y/8 * 32 + 0x2000];
    }

    if (attributes & 0x80) {
        priority = !bg_behind && bg_enabled;
    }

    if (!priority && sprite_pixel) {
        if (!gb->cgb_mode) {
            sprite_pixel = (gb->io_registers[use_obp1? GB_IO_OBP1:GB_IO_OBP0] >> (sprite_pixel << 1)) & 3;
            sprite_palette = use_obp1;
        }
        return gb->sprite_palletes_rgb[sprite_palette * 4 + sprite_pixel];
    }

    if (bg_enabled) {
        if (gb->io_registers[GB_IO_LCDC] & 0x10) {
            tile_address = tile * 0x10;
        }
        else {
            tile_address = (int8_t) tile * 0x10 + 0x1000;
        }
        if (attributes & 0x8) {
            tile_address += 0x2000;
        }

        if (attributes & 0x20) {
            x = ~x;
        }

        if (attributes & 0x40) {
            y = ~y;
        }

        background_pixel = (((gb->vram[tile_address + (y & 7) * 2    ] >> ((~x)&7)) & 1 ) |
                            ((gb->vram[tile_address + (y & 7) * 2 + 1] >> ((~x)&7)) & 1) << 1 );
    }

    if (priority && sprite_pixel && !background_pixel) {
        if (!gb->cgb_mode) {
            sprite_pixel = (gb->io_registers[use_obp1? GB_IO_OBP1:GB_IO_OBP0] >> (sprite_pixel << 1)) & 3;
            sprite_palette = use_obp1;
        }
        return gb->sprite_palletes_rgb[sprite_palette * 4 + sprite_pixel];
    }

    if (!gb->cgb_mode) {
        background_pixel = ((gb->io_registers[GB_IO_BGP] >> (background_pixel << 1)) & 3);
    }

    return gb->background_palletes_rgb[(attributes & 7) * 4 + background_pixel];
}

static int64_t get_nanoseconds(void)
{
#ifndef _WIN32
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_usec) * 1000 + now.tv_sec * 1000000000L;
#else
    FILETIME time;
	GetSystemTimeAsFileTime(&time);
	return (((int64_t)time.dwHighDateTime << 32) | time.dwLowDateTime) * 100L;
#endif
}

static void nsleep(uint64_t nanoseconds)
{
#ifndef _WIN32
    struct timespec sleep = {0, nanoseconds};
    nanosleep(&sleep, NULL);
#else
	HANDLE timer;
	LARGE_INTEGER time;
	timer = CreateWaitableTimer(NULL, true, NULL);
	time.QuadPart = -(nanoseconds / 100L);
	SetWaitableTimer(timer, &time, 0, NULL, NULL, false);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
#endif
}

// Todo: FPS capping should not be related to vblank, as the display is not always on, and this causes "jumps"
// when switching the display on and off.
void display_vblank(GB_gameboy_t *gb)
{
    /* Called every Gameboy vblank. Does FPS-capping and calls user's vblank callback if Turbo Mode allows. */
    if (gb->turbo && !gb->turbo_dont_skip) {
        int64_t nanoseconds = get_nanoseconds();
        if (nanoseconds <= gb->last_vblank + FRAME_LENGTH) {
            return;
        }
        gb->last_vblank = nanoseconds;
    }

    /*
    static long start = 0;
    static long last = 0;
    static long frames = 0;

    if (last == 0) {
        last = time(NULL);
    }

    if (last != time(NULL)) {
        last = time(NULL);
        if (start == 0) {
            start = last;
            frames = 0;
        }
        printf("Average FPS: %f\n", frames / (double)(last - start));
    }
    frames++;
     */

    if (!(gb->io_registers[GB_IO_LCDC] & 0x80) || gb->stopped) {
        /* LCD is off, memset screen to white */
        memset(gb->screen, 0xFF, WIDTH * LINES * 4);
    }

    gb->vblank_callback(gb);
    if (!gb->turbo) {
        int64_t nanoseconds = get_nanoseconds();
        if (labs((signed long)(nanoseconds - gb->last_vblank)) < FRAME_LENGTH ) {
            nsleep(FRAME_LENGTH  + gb->last_vblank - nanoseconds);
            gb->last_vblank += FRAME_LENGTH;
        }
        else {
            gb->last_vblank = nanoseconds;
        }
    }

    gb->vblank_just_occured = true;
}

static inline uint8_t scale_channel(uint8_t x)
{
    x &= 0x1f;
    return (x << 3) | (x >> 2);
}

void GB_palette_changed(GB_gameboy_t *gb, bool background_palette, uint8_t index)
{
    uint8_t *palette_data = background_palette? gb->background_palletes_data : gb->sprite_palletes_data;
    uint16_t color = palette_data[index & ~1] | (palette_data[index | 1] << 8);

    // No need to &, scale channel does that.
    uint8_t r = scale_channel(color);
    uint8_t g = scale_channel(color >> 5);
    uint8_t b = scale_channel(color >> 10);
    assert (gb->rgb_encode_callback);
    (background_palette? gb->background_palletes_rgb : gb->sprite_palletes_rgb)[index / 2] = gb->rgb_encode_callback(gb, r, g, b);
}

/*
 STAT interrupt is implemented based on this finding:
 http://board.byuu.org/phpbb3/viewtopic.php?p=25527#p25531
 
 General timing is based on GiiBiiAdvance's documents:
 https://github.com/AntonioND/giibiiadvance
 
 */

static void update_display_state(GB_gameboy_t *gb, uint8_t cycles)
{
    uint8_t previous_stat_interrupt_line = gb->stat_interrupt_line;
    gb->stat_interrupt_line = false;
    
    if (!(gb->io_registers[GB_IO_LCDC] & 0x80)) {
        /* LCD is disabled, state is constant */
        
        /* When the LCD is off, LY is 0 and STAT mode is 0.
           Todo: Verify the LY=LYC flag should be on. */
        gb->io_registers[GB_IO_LY] = 0;
        gb->io_registers[GB_IO_STAT] &= ~3;
        gb->io_registers[GB_IO_STAT] |= 4;
        if (gb->hdma_on_hblank) {
            gb->hdma_on_hblank = false;
            gb->hdma_on = false;
            
            /* Todo: is this correct? */
            gb->hdma_steps_left = 0xff;
        }
        
        /* Keep sending vblanks to user even if the screen is off */
        gb->display_cycles += cycles;
        if (gb->display_cycles >= LCDC_PERIOD) {
            /* VBlank! */
            gb->display_cycles -= LCDC_PERIOD;
            display_vblank(gb);
        }

        /* Reset window rendering state */
        gb->effective_window_enabled = false;
        gb->effective_window_y = 0xFF;
        return;
    }
    
    uint8_t atomic_increase = gb->cgb_double_speed? 2 : 4;
    uint8_t stat_delay = gb->cgb_double_speed? 2 : (gb->cgb_mode? 0 : 4);
    
    for (; cycles; cycles -= atomic_increase) {
        gb->display_cycles += atomic_increase;
        bool should_compare_ly = true;
        uint8_t ly_for_comparison = gb->io_registers[GB_IO_LY] = gb->display_cycles / LINE_LENGTH;
        
        /* Handle cycle completion. STAT's initial value depends on model and mode */
        if (gb->display_cycles == LCDC_PERIOD) {
            /* VBlank! */
            gb->display_cycles = 0;
            gb->io_registers[GB_IO_STAT] &= ~3;
            if (gb->is_cgb) {
                if (stat_delay) {
                    gb->io_registers[GB_IO_STAT] |= 1;
                }
                else {
                    gb->io_registers[GB_IO_STAT] |= 2;
                }
            }
            ly_for_comparison = gb->io_registers[GB_IO_LY] = 0;
            
            /* Reset window rendering state */
            gb->effective_window_enabled = false;
            gb->effective_window_y = 0xFF;
            display_vblank(gb);
        }
        
        /* Entered VBlank state, update STAT and IF */
        else if (gb->display_cycles == LINES * LINE_LENGTH + stat_delay) {
            gb->io_registers[GB_IO_STAT] &= ~3;
            gb->io_registers[GB_IO_STAT] |= 1;
            gb->io_registers[GB_IO_IF] |= 1;
            
            /* Entering VBlank state triggers the OAM interrupt */
            if (gb->io_registers[GB_IO_STAT] & 0x20) {
                gb->stat_interrupt_line = true;
            }
        }
        
        /* Handle STAT changes for lines 0-143 */
        else if (gb->display_cycles < LINES * LINE_LENGTH ) {
            unsigned position_in_line = gb->display_cycles % LINE_LENGTH;
            if (position_in_line == stat_delay) {
                gb->io_registers[GB_IO_STAT] &= ~3;
                gb->io_registers[GB_IO_STAT] |= 2;
            }
            else if (position_in_line == 0 && gb->display_cycles != 0) {
                should_compare_ly = gb->is_cgb;
                ly_for_comparison--;
            }
            else if (position_in_line == MODE2_LENGTH + stat_delay) {
                gb->io_registers[GB_IO_STAT] &= ~3;
                gb->io_registers[GB_IO_STAT] |= 3;
            }
            else if (position_in_line == MODE2_LENGTH + MODE3_LENGTH + stat_delay) {
                gb->io_registers[GB_IO_STAT] &= ~3;
                if (gb->hdma_on_hblank) {
                    gb->hdma_on = true;
                    gb->hdma_cycles = 0;
                }
            }
        }
        
        /* Line 153 is special */
        else if (gb->display_cycles >= (VIRTUAL_LINES - 1) * LINE_LENGTH) {
            /* DMG */
            if (!gb->is_cgb) {
                switch (gb->display_cycles - (VIRTUAL_LINES - 1) * LINE_LENGTH) {
                    case 0:
                        should_compare_ly = false;
                        break;
                    case 4:
                        gb->io_registers[GB_IO_LY] = 0;
                        ly_for_comparison = VIRTUAL_LINES - 1;
                        break;
                    case 8:
                        gb->io_registers[GB_IO_LY] = 0;
                        should_compare_ly = false;
                        break;
                    default:
                        gb->io_registers[GB_IO_LY] = 0;
                        ly_for_comparison = 0;
                }
            }
            /* CGB in DMG mode */
            else if (!gb->cgb_mode) {
                switch (gb->display_cycles - (VIRTUAL_LINES - 1) * LINE_LENGTH) {
                    case 0:
                        ly_for_comparison = VIRTUAL_LINES - 2;
                        break;
                    case 4:
                        break;
                    case 8:
                        gb->io_registers[GB_IO_LY] = 0;
                        break;
                    default:
                        gb->io_registers[GB_IO_LY] = 0;
                        ly_for_comparison = 0;
                }
            }
            /* Single speed CGB */
            else if (!gb->cgb_double_speed) {
                switch (gb->display_cycles - (VIRTUAL_LINES - 1) * LINE_LENGTH) {
                    case 0:
                        break;
                    case 4:
                        gb->io_registers[GB_IO_LY] = 0;
                        break;
                    default:
                        gb->io_registers[GB_IO_LY] = 0;
                        ly_for_comparison = 0;
                }
            }
            
            /* Double speed CGB */
            else {
                switch (gb->display_cycles - (VIRTUAL_LINES - 1) * LINE_LENGTH) {
                    case 0:
                        ly_for_comparison = VIRTUAL_LINES - 2;
                        break;
                    case 2:
                    case 4:
                        break;
                    case 6:
                    case 8:
                        gb->io_registers[GB_IO_LY] = 0;
                        break;
                    default:
                        gb->io_registers[GB_IO_LY] = 0;
                        ly_for_comparison = 0;
                }
            }
        }
        
        /* Lines 144 - 152 */
        else {
            if (stat_delay && gb->display_cycles % LINE_LENGTH == 0) {
                should_compare_ly = gb->is_cgb;
                ly_for_comparison--;
            }
        }
        
        /* Set LY=LYC bit */
        if (should_compare_ly && (ly_for_comparison == gb->io_registers[GB_IO_LYC])) {
            gb->io_registers[GB_IO_STAT] |= 4;
        }
        else {
            gb->io_registers[GB_IO_STAT] &= ~4;
        }
        
        if (!gb->stat_interrupt_line) {
            switch (gb->io_registers[GB_IO_STAT] & 3) {
                case 0: gb->stat_interrupt_line = gb->io_registers[GB_IO_STAT] & 8; break;
                case 1: gb->stat_interrupt_line = gb->io_registers[GB_IO_STAT] & 0x10; break;
                case 2: gb->stat_interrupt_line = gb->io_registers[GB_IO_STAT] & 0x20; break;
            }
            
            /* Use requested a LY=LYC interrupt and the LY=LYC bit is on */
            if ((gb->io_registers[GB_IO_STAT] & 0x44) == 0x44) {
                gb->stat_interrupt_line = true;
            }
        }
        
        if (!gb->stat_interrupt_line) {
        }
    }
    
    if (gb->stat_interrupt_line && !previous_stat_interrupt_line) {
        gb->io_registers[GB_IO_IF] |= 2;
    }
    
    /* The value of LY is glitched in the last cycle of every line in CGB mode CGB in single speed
       This is based on GiiBiiAdvance's docs */
    if (gb->cgb_mode && !gb->cgb_double_speed &&
        gb->display_cycles % LINE_LENGTH == LINE_LENGTH - 4) {
        uint8_t glitch_pattern[] = {0, 0, 2, 0, 4, 4, 6, 0, 8};
        if ((gb->io_registers[GB_IO_LY] & 0xF) == 0xF) {
            gb->io_registers[GB_IO_LY] = glitch_pattern[gb->io_registers[GB_IO_LY] >> 4] << 4;
        }
        else {
            gb->io_registers[GB_IO_LY] = glitch_pattern[gb->io_registers[GB_IO_LY] & 7] | (gb->io_registers[GB_IO_LY] & 0xF8);
        }
    }
}

void GB_display_run(GB_gameboy_t *gb, uint8_t cycles)
{
    update_display_state(gb, cycles);
    if (gb->disable_rendering) {
        return;
    }
    
    /*
       Display controller bug: For some reason, the OAM STAT interrupt is called, as expected, for LY = 0..143.
       However, it is also called from LY = 144.

       See http://forums.nesdev.com/viewtopic.php?f=20&t=13727
    */

    if (!(gb->io_registers[GB_IO_LCDC] & 0x80)) {
        /* LCD is disabled, do nothing */
        return;
    }
    if (gb->display_cycles >= LINE_LENGTH * 144) { /* VBlank */
        return;
    }
    
    uint8_t effective_ly = gb->display_cycles / LINE_LENGTH;

    // Todo: verify this window behavior. It is assumed from the expected behavior of 007 - The World Is Not Enough.
    if ((gb->io_registers[GB_IO_LCDC] & 0x20) && effective_ly == gb->io_registers[GB_IO_WY]) {
        gb->effective_window_enabled = true;
    }

    if (gb->display_cycles % LINE_LENGTH < MODE2_LENGTH) { /* Mode 2 */

        /* See above comment about window behavior. */
        if (gb->effective_window_enabled && gb->effective_window_y == 0xFF) {
            gb->effective_window_y = effective_ly;
        }

        gb->effective_scx = gb->io_registers[GB_IO_SCX];
        gb->previous_lcdc_x = - (gb->effective_scx & 0x7);
        return;
    }


    /* Render */
    int16_t current_lcdc_x = ((gb->display_cycles % LINE_LENGTH - MODE2_LENGTH) & ~7) - (gb->effective_scx & 0x7);
    
    for (;gb->previous_lcdc_x < current_lcdc_x; gb->previous_lcdc_x++) {
        if (gb->previous_lcdc_x >= WIDTH) {
            continue;
        }
        if (gb->previous_lcdc_x < 0) {
            continue;
        }
        gb->screen[effective_ly * WIDTH + gb->previous_lcdc_x] =
        get_pixel(gb, gb->previous_lcdc_x, effective_ly);
    }
}

void GB_draw_tileset(GB_gameboy_t *gb, uint32_t *dest, GB_palette_type_t palette_type, uint8_t palette_index)
{
    uint32_t none_palette[4];
    uint32_t *palette = NULL;
    
    switch (gb->is_cgb? palette_type : GB_PALETTE_NONE) {
        default:
        case GB_PALETTE_NONE:
            none_palette[0] = gb->rgb_encode_callback(gb, 0xFF, 0xFF, 0xFF);
            none_palette[1] = gb->rgb_encode_callback(gb, 0xAA, 0xAA, 0xAA);
            none_palette[2] = gb->rgb_encode_callback(gb, 0x55, 0x55, 0x55);
            none_palette[3] = gb->rgb_encode_callback(gb, 0,    0,    0   );
            palette = none_palette;
            break;
        case GB_PALETTE_BACKGROUND:
            palette = gb->background_palletes_rgb + (4 * (palette_index & 7));
            break;
        case GB_PALETTE_OAM:
            palette = gb->sprite_palletes_rgb + (4 * (palette_index & 7));
            break;
    }
    
    for (unsigned y = 0; y < 192; y++) {
        for (unsigned x = 0; x < 256; x++) {
            if (x >= 128 && !gb->is_cgb) {
                *(dest++) = gb->background_palletes_rgb[0];
                continue;
            }
            uint16_t tile = (x % 128) / 8 + y / 8 * 16;
            uint16_t tile_address = tile * 0x10 + (x >= 128? 0x2000 : 0);
            uint8_t pixel = (((gb->vram[tile_address + (y & 7) * 2    ] >> ((~x)&7)) & 1 ) |
                             ((gb->vram[tile_address + (y & 7) * 2 + 1] >> ((~x)&7)) & 1) << 1);
            
            if (!gb->cgb_mode) {
                if (palette_type == GB_PALETTE_BACKGROUND) {
                    pixel = ((gb->io_registers[GB_IO_BGP] >> (pixel << 1)) & 3);
                }
                else if (!gb->cgb_mode) {
                    if (palette_type == GB_PALETTE_OAM) {
                        pixel = ((gb->io_registers[palette_index == 0? GB_IO_OBP0 : GB_IO_OBP1] >> (pixel << 1)) & 3);
                    }
                }
            }
            
            
            *(dest++) = palette[pixel];
        }
    }
}

void GB_draw_tilemap(GB_gameboy_t *gb, uint32_t *dest, GB_palette_type_t palette_type, uint8_t palette_index, GB_map_type_t map_type, GB_tileset_type_t tileset_type)
{
    uint32_t none_palette[4];
    uint32_t *palette = NULL;
    uint16_t map = 0x1800;
    
    switch (gb->is_cgb? palette_type : GB_PALETTE_NONE) {
        case GB_PALETTE_NONE:
            none_palette[0] = gb->rgb_encode_callback(gb, 0xFF, 0xFF, 0xFF);
            none_palette[1] = gb->rgb_encode_callback(gb, 0xAA, 0xAA, 0xAA);
            none_palette[2] = gb->rgb_encode_callback(gb, 0x55, 0x55, 0x55);
            none_palette[3] = gb->rgb_encode_callback(gb, 0,    0,    0   );
            palette = none_palette;
            break;
        case GB_PALETTE_BACKGROUND:
            palette = gb->background_palletes_rgb + (4 * (palette_index & 7));
            break;
        case GB_PALETTE_OAM:
            palette = gb->sprite_palletes_rgb + (4 * (palette_index & 7));
            break;
        case GB_PALETTE_AUTO:
            break;
    }
    
    if (map_type == GB_MAP_9C00 || (map_type == GB_MAP_AUTO && gb->io_registers[GB_IO_LCDC] & 0x08)) {
        map = 0x1c00;
    }
    
    if (tileset_type == GB_TILESET_AUTO) {
        tileset_type = (gb->io_registers[GB_IO_LCDC] & 0x10)? GB_TILESET_8800 : GB_TILESET_8000;
    }
    
    for (unsigned y = 0; y < 256; y++) {
        for (unsigned x = 0; x < 256; x++) {
            uint8_t tile = gb->vram[map + x/8 + y/8 * 32];
            uint16_t tile_address;
            uint8_t attributes = 0;
            
            if (tileset_type == GB_TILESET_8800) {
                tile_address = tile * 0x10;
            }
            else {
                tile_address = (int8_t) tile * 0x10 + 0x1000;
            }
            
            if (gb->cgb_mode) {
                attributes = gb->vram[map + x/8 + y/8 * 32 + 0x2000];
            }
            
            if (attributes & 0x8) {
                tile_address += 0x2000;
            }
            
            uint8_t pixel = (((gb->vram[tile_address + (((attributes & 0x40)? ~y : y) & 7) * 2    ] >> (((attributes & 0x20)? x : ~x)&7)) & 1 ) |
                             ((gb->vram[tile_address + (((attributes & 0x40)? ~y : y) & 7) * 2 + 1] >> (((attributes & 0x20)? x : ~x)&7)) & 1) << 1);
            
            if (!gb->cgb_mode && (palette_type == GB_PALETTE_BACKGROUND || palette_type == GB_PALETTE_AUTO)) {
                pixel = ((gb->io_registers[GB_IO_BGP] >> (pixel << 1)) & 3);
            }
            
            if (palette) {
                *(dest++) = palette[pixel];
            }
            else {
                *(dest++) = gb->background_palletes_rgb[(attributes & 7) * 4 + pixel];
            }
        }
    }
}

uint8_t GB_get_oam_info(GB_gameboy_t *gb, GB_oam_info_t *dest)
{
    uint8_t count = 0;
    unsigned sprite_height = (gb->io_registers[GB_IO_LCDC] & 4) ? 16:8;
    uint8_t oam_to_dest_index[40] = {0,};
    for (unsigned y = 0; y < LINES; y++) {
        GB_sprite_t *sprite = (GB_sprite_t *) &gb->oam;
        uint8_t sprites_in_line = 0;
        for (uint8_t i = 0; i < 40; i++, sprite++) {
            int sprite_y = sprite->y - 16;
            bool obscured = false;
            // Is sprite not in this line?
            if (sprite_y > y || sprite_y + sprite_height <= y) continue;
            if (++sprites_in_line == 11) obscured = true;
            
            GB_oam_info_t *info = NULL;
            if (!oam_to_dest_index[i]) {
                info = dest + count;
                oam_to_dest_index[i] = ++count;
                info->x = sprite->x;
                info->y = sprite->y;
                info->tile = sprite_height == 16? sprite->tile & 0xFE : sprite->tile;
                info->flags = sprite->flags;
                info->obscured_by_line_limit = false;
                info->oam_addr = 0xFE00 + i * sizeof(*sprite);
            }
            else {
                info = dest + oam_to_dest_index[i] - 1;
            }
            info->obscured_by_line_limit |= obscured;
        }
    }
    
    
    for (unsigned i = 0; i < count; i++) {
        uint16_t vram_address = dest[i].tile * 0x10;
        uint8_t flags = dest[i].flags;
        uint8_t palette = gb->cgb_mode? (flags & 7) : ((flags & 0x10)? 1 : 0);

        for (unsigned y = 0; y < sprite_height; y++) {
            for (unsigned x = 0; x < 8; x++) {
                uint8_t color = (((gb->vram[vram_address    ] >> ((~x)&7)) & 1 ) |
                                 ((gb->vram[vram_address + 1] >> ((~x)&7)) & 1) << 1 );
                
                if (!gb->cgb_mode) {
                    color = (gb->io_registers[palette? GB_IO_OBP1:GB_IO_OBP0] >> (color << 1)) & 3;
                }
                dest[i].image[((flags & 0x20)?7-x:x) + ((flags & 0x40)?sprite_height - 1 -y:y) * 8] = gb->sprite_palletes_rgb[palette * 4 + color];
            }
            vram_address += 2;
        }
    }
    return count;
}
