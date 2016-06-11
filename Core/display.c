#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "gb.h"
#include "display.h"

#pragma pack(push, 1)
typedef struct {
    unsigned char y;
    unsigned char x;
    unsigned char tile;
    unsigned char flags;
} GB_sprite_t;
#pragma pack(pop)

static uint32_t get_pixel(GB_gameboy_t *gb, unsigned char x, unsigned char y)
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
    unsigned short map = 0x1800;
    unsigned char tile = 0;
    unsigned char attributes = 0;
    unsigned char sprite_palette = 0;
    unsigned short tile_address = 0;
    unsigned char background_pixel = 0, sprite_pixel = 0;
    GB_sprite_t *sprite = (GB_sprite_t *) &gb->oam;
    unsigned char sprites_in_line = 0;
    bool lcd_8_16_mode = (gb->io_registers[GB_IO_LCDC] & 4) != 0;
    bool sprites_enabled = (gb->io_registers[GB_IO_LCDC] & 2) != 0;
    unsigned char lowest_sprite_x = 0xFF;
    bool use_obp1 = false, priority = false;
    bool in_window = false;
    if (gb->effective_window_enabled && (gb->io_registers[GB_IO_LCDC] & 0x20)) { /* Window Enabled */
        if (y >= gb->effective_window_y && x + 7 >= gb->io_registers[GB_IO_WX]) {
            in_window = true;
        }
    }

    if (sprites_enabled) {
        // Loop all sprites
        for (unsigned char i = 40; i--; sprite++) {
            int sprite_y = sprite->y - 16;
            int sprite_x = sprite->x - 8;
            // Is sprite in our line?
            if (sprite_y <= y && sprite_y + (lcd_8_16_mode? 16:8) > y) {
                unsigned char tile_x, tile_y, current_sprite_pixel;
                unsigned short line_address;
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
        priority = true;
    }

    if (!priority && sprite_pixel) {
        if (!gb->cgb_mode) {
            sprite_pixel = (gb->io_registers[use_obp1? GB_IO_OBP1:GB_IO_OBP0] >> (sprite_pixel << 1)) & 3;
            sprite_palette = use_obp1;
        }
        return gb->sprite_palletes_rgb[sprite_palette * 4 + sprite_pixel];
    }

    if (gb->io_registers[GB_IO_LCDC] & 0x10) {
        tile_address = tile * 0x10;
    }
    else {
        tile_address = (signed char) tile * 0x10 + 0x1000;
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

// Todo: FPS capping should not be related to vblank, as the display is not always on, and this causes "jumps"
// when switching the display on and off.
void display_vblank(GB_gameboy_t *gb)
{
    _Static_assert(CLOCKS_PER_SEC == 1000000, "CLOCKS_PER_SEC != 1000000");
    
    /* Called every Gameboy vblank. Does FPS-capping and calls user's vblank callback if Turbo Mode allows. */
    if (gb->turbo) {
        struct timeval now;
        gettimeofday(&now, NULL);
        signed long nanoseconds = (now.tv_usec) * 1000 + now.tv_sec * 1000000000L;
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

    if (!(gb->io_registers[GB_IO_LCDC] & 0x80)) {
        /* LCD is off, memset screen to white */
        memset(gb->screen, 0xFF, 160 * 144 * 4);
    }

    gb->vblank_callback(gb);
    if (!gb->turbo) {
        struct timeval now;
        struct timespec sleep = {0,};
        gettimeofday(&now, NULL);
        signed long nanoseconds = (now.tv_usec) * 1000 + now.tv_sec * 1000000000L;
        if (labs(nanoseconds - gb->last_vblank) < FRAME_LENGTH ) {
            sleep.tv_nsec = (FRAME_LENGTH  + gb->last_vblank - nanoseconds);
            nanosleep(&sleep, NULL);

            gb->last_vblank += FRAME_LENGTH;
        }
        else {
            gb->last_vblank = nanoseconds;
        }
    }
}

static inline unsigned char scale_channel(unsigned char x)
{
    x &= 0x1f;
    return (x << 3) | (x >> 2);
}

void palette_changed(GB_gameboy_t *gb, bool background_palette, unsigned char index)
{
    unsigned char *palette_data = background_palette? gb->background_palletes_data : gb->sprite_palletes_data;
    unsigned short color = palette_data[index & ~1] | (palette_data[index | 1] << 8);

    // No need to &, scale channel does that.
    unsigned char r = scale_channel(color);
    unsigned char g = scale_channel(color >> 5);
    unsigned char b = scale_channel(color >> 10);
    assert (gb->rgb_encode_callback);
    (background_palette? gb->background_palletes_rgb : gb->sprite_palletes_rgb)[index / 2] = gb->rgb_encode_callback(gb, r, g, b);
}

void display_run(GB_gameboy_t *gb)
{
    /*
       Display controller bug: For some reason, the OAM STAT interrupt is called, as expected, for LY = 0..143.
       However, it is also called from LY = 144.

       See http://forums.nesdev.com/viewtopic.php?f=20&t=13727
    */
    unsigned char last_mode = gb->io_registers[GB_IO_STAT] & 3;

    /*
        STAT interrupt is implemented based on this finding:
        http://board.byuu.org/phpbb3/viewtopic.php?p=25527#p25531
     */
    unsigned char previous_stat_interrupt_line = gb->stat_interrupt_line;
    gb->stat_interrupt_line = false;

    if (gb->display_cycles >= LCDC_PERIOD) {
        /* VBlank! */
        gb->display_cycles -= LCDC_PERIOD;
        gb->ly144_bug_oam = false;
        gb->ly144_bug_hblank = false;
        display_vblank(gb);
    }

    if (!(gb->io_registers[GB_IO_LCDC] & 0x80)) {
        /* LCD is disabled, do nothing */
        gb->io_registers[GB_IO_STAT] &= ~3;
        gb->io_registers[GB_IO_LY] = 0;
        return;
    }

    gb->io_registers[GB_IO_STAT] &= ~3;

    /*
     Each line is 456 cycles, approximately:
     Mode 2 - 80 cycles
     Mode 3 - 172 cycles
     Mode 0 - 204 cycles
     
     Todo: Mode lengths are not constants???
     */

    gb->io_registers[GB_IO_LY] = gb->display_cycles / 456;

    gb->io_registers[GB_IO_STAT] &= ~4;
    if (gb->io_registers[GB_IO_LY] == gb->io_registers[GB_IO_LYC]) {
        gb->io_registers[GB_IO_STAT] |= 4;
        if (gb->io_registers[GB_IO_STAT] & 0x40) {
            /* User requests LYC interrupt. */
            gb->stat_interrupt_line = true;
        }
    }

    /* Todo: This behavior is seen in BGB and it fixes some ROMs with delicate timing, such as Hitman's 8bit.
       This should be verified to be correct on a real gameboy. */
    if (gb->io_registers[GB_IO_LY] == 153 && gb->display_cycles % 456 > 8) {
        gb->io_registers[GB_IO_LY] = 0;
    }

    if (gb->display_cycles >= 456 * 144) { /* VBlank */
        gb->io_registers[GB_IO_STAT] |= 1; /* Set mode to 1 */
        gb->effective_window_enabled = false;
        gb->effective_window_y = 0xFF;

        if (gb->io_registers[GB_IO_STAT] & 16) { /* User requests an interrupt on VBlank*/
            gb->stat_interrupt_line = true;
        }
        if (last_mode != 1) {
            gb->io_registers[GB_IO_IF] |= 1;
        }

        // LY = 144 interrupt bug
        if (gb->io_registers[GB_IO_LY] == 144) {
            if (gb->display_cycles % 456 < 80) { // Mode 2
                if (gb->io_registers[GB_IO_STAT] & 0x20 && !gb->ly144_bug_oam) { /* User requests an interrupt on Mode 2 */
                    gb->io_registers[GB_IO_IF] |= 2;
                }
                gb->ly144_bug_oam = true;
            }
            if (gb->display_cycles % 456 < 80 + 172) { /* Mode 3 */
                // Nothing to do
            }
            else { /* Mode 0 */
                if (gb->io_registers[GB_IO_STAT] & 8 && !gb->ly144_bug_hblank) { /* User requests an interrupt on Mode 0 */
                    /*
                    Todo: Verify if this actually happens.
                    gb->io_registers[GB_IO_IF] |= 2;
                    */
                }
                gb->ly144_bug_hblank = true;
            }
        }

        goto updateSTAT;
    }

    // Todo: verify this window behavior. It is assumed from the expected behavior of 007 - The World Is Not Enough.
    if ((gb->io_registers[GB_IO_LCDC] & 0x20) && gb->io_registers[GB_IO_LY] == gb->io_registers[GB_IO_WY]) {
        gb->effective_window_enabled = true;
    }

    if (gb->display_cycles % 456 < 80) { /* Mode 2 */
        gb->io_registers[GB_IO_STAT] |= 2; /* Set mode to 2 */

        if (gb->io_registers[GB_IO_STAT] & 0x20) { /* User requests an interrupt on Mode 2 */
            gb->stat_interrupt_line = true;
        }

        /* See above comment about window behavior. */
        if (gb->effective_window_enabled && gb->effective_window_y == 0xFF) {
            gb->effective_window_y =  gb->io_registers[GB_IO_LY];
        }

        gb->effective_scx = gb->io_registers[GB_IO_SCX];
        gb->previous_lcdc_x = - (gb->effective_scx & 0x7);
        goto updateSTAT;
    }

    signed short current_lcdc_x = ((gb->display_cycles % 456 - 80) & ~7) - (gb->effective_scx & 0x7);
    for (;gb->previous_lcdc_x < current_lcdc_x; gb->previous_lcdc_x++) {
        if (gb->previous_lcdc_x >= 160) {
            continue;
        }
        if (gb->previous_lcdc_x < 0) {
            continue;
        }
        gb->screen[gb->io_registers[GB_IO_LY] * 160 + gb->previous_lcdc_x] =
        get_pixel(gb, gb->previous_lcdc_x, gb->io_registers[GB_IO_LY]);
    }

    if (gb->display_cycles % 456 < 80 + 172) { /* Mode 3 */
        gb->io_registers[GB_IO_STAT] |= 3; /* Set mode to 3 */
        goto updateSTAT;
    }

    /* if (gb->display_cycles % 456 < 80 + 172 + 204) */ { /* Mode 0*/
        if (gb->io_registers[GB_IO_STAT] & 8) { /* User requests an interrupt on Mode 0 */
            gb->stat_interrupt_line = true;
        }
        if (last_mode != 0) {
            if (gb->hdma_on_hblank) {
                gb->hdma_on = true;
                gb->hdma_cycles = 0;
            }
        }
    }

updateSTAT:
    if (gb->stat_interrupt_line && !previous_stat_interrupt_line) {
        gb->io_registers[GB_IO_IF] |= 2;
    }
}
