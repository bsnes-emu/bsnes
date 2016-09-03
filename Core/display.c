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

#pragma pack(push, 1)
typedef struct {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    uint8_t flags;
} GB_sprite_t;
#pragma pack(pop)

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
    if (gb->effective_window_enabled && (gb->io_registers[GB_IO_LCDC] & 0x20)) { /* Window Enabled */
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
    if (gb->turbo) {
        int64_t nanoseconds = get_nanoseconds();
        if (!gb->turbo_dont_skip && nanoseconds <= gb->last_vblank + FRAME_LENGTH) {
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
        memset(gb->screen, 0xFF, 160 * 144 * 4);
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
 Each line is 456 cycles, approximately:
 Mode 2 - 80  cycles / OAM Transfer
 Mode 3 - 172 cycles / Rendering
 Mode 0 - 204 cycles / HBlank
 
 Mode 1 is VBlank

 Todo: Mode lengths are not constants, see http://blog.kevtris.org/blogfiles/Nitty%20Gritty%20Gameboy%20VRAM%20Timing.txt
 */

#define MODE2_LENGTH 80
#define MODE3_LENGTH 172
#define MODE1_LENGTH 204
#define LINE_LENGTH (MODE2_LENGTH + MODE3_LENGTH + MODE1_LENGTH) // = 456

void GB_display_run(GB_gameboy_t *gb)
{
    /*
       Display controller bug: For some reason, the OAM STAT interrupt is called, as expected, for LY = 0..143.
       However, it is also called from LY = 144.

       See http://forums.nesdev.com/viewtopic.php?f=20&t=13727
    */

    /*
        STAT interrupt is implemented based on this finding:
        http://board.byuu.org/phpbb3/viewtopic.php?p=25527#p25531
     */

    uint8_t previous_stat_interrupt_line = gb->stat_interrupt_line;
    gb->stat_interrupt_line = false;

    uint8_t last_mode = gb->io_registers[GB_IO_STAT] & 3;
    gb->io_registers[GB_IO_STAT] &= ~3;

    if (gb->display_cycles >= LCDC_PERIOD) {
        /* VBlank! */
        gb->display_cycles -= LCDC_PERIOD;
        display_vblank(gb);
    }

    if (!(gb->io_registers[GB_IO_LCDC] & 0x80)) {
        /* LCD is disabled, do nothing */

        /* Some games expect LY to be zero when the LCD is off.
           Todo: Verify this behavior.
         Keep in mind that this only affects the value being read from the Gameboy, not the actualy display state. 
         This also explains why the coincidence interrupt triggers when LYC = 0 and LY = 153. */
        gb->io_registers[GB_IO_LY] = 0;
        return;
    }


    gb->io_registers[GB_IO_LY] = gb->display_cycles / LINE_LENGTH;

    /* Todo: This behavior is seen in BGB and it fixes some ROMs with delicate timing, such as Hitman's 8bit.
       This should be verified to be correct on a real gameboy. */
    if (gb->io_registers[GB_IO_LY] == 153 && gb->display_cycles % LINE_LENGTH > 8) {
        gb->io_registers[GB_IO_LY] = 0;
    }

    gb->io_registers[GB_IO_STAT] &= ~4;
    if (gb->io_registers[GB_IO_LY] == gb->io_registers[GB_IO_LYC]) {
        gb->io_registers[GB_IO_STAT] |= 4;
        if (gb->io_registers[GB_IO_STAT] & 0x40) {
            /* User requests LYC interrupt. */
            gb->stat_interrupt_line = true;
        }
    }

    if (gb->display_cycles >= LINE_LENGTH * 144) { /* VBlank */
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
            /* User requests an interrupt on Mode 2 */
            if (gb->display_cycles % LINE_LENGTH < MODE2_LENGTH && gb->io_registers[GB_IO_STAT] & 0x20) { // Mode 2
                gb->stat_interrupt_line = true;
            }
        }

        goto updateSTAT;
    }

    // Todo: verify this window behavior. It is assumed from the expected behavior of 007 - The World Is Not Enough.
    if ((gb->io_registers[GB_IO_LCDC] & 0x20) && gb->io_registers[GB_IO_LY] == gb->io_registers[GB_IO_WY]) {
        gb->effective_window_enabled = true;
    }

    if (gb->display_cycles % LINE_LENGTH < MODE2_LENGTH) { /* Mode 2 */
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


    /* Render. This  chunk is outside the Mode 3 if, because otherwise we might not render some pixels, since this
       function only runs between atomic CPU changes, and not every clock. */
    if (!gb->disable_rendering) {
        int16_t current_lcdc_x = ((gb->display_cycles % LINE_LENGTH - MODE2_LENGTH) & ~7) - (gb->effective_scx & 0x7);
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
    }

    if (gb->display_cycles % LINE_LENGTH < MODE2_LENGTH + MODE3_LENGTH) { /* Mode 3 */
        gb->io_registers[GB_IO_STAT] |= 3; /* Set mode to 3 */
        goto updateSTAT;
    }

     /* Mode 0*/
    if (gb->io_registers[GB_IO_STAT] & 8) { /* User requests an interrupt on Mode 0 */
        gb->stat_interrupt_line = true;
    }

    if (last_mode != 0) {
        if (gb->hdma_on_hblank) {
            gb->hdma_on = true;
            gb->hdma_cycles = 0;
        }
    }

updateSTAT:
    if (gb->stat_interrupt_line && !previous_stat_interrupt_line) {
        gb->io_registers[GB_IO_IF] |= 2;
    }
}
