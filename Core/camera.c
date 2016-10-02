#include "camera.h"

static int8_t dither_random(uint8_t x, uint8_t y)
{
    static bool once = false;
    static int8_t random[128*112];
    if (!once) {
        unsigned int r = 0x1337c0de;
        for (int i = 0; i < sizeof(random); i++) {
            random[i] = r % 85 - 42;
            r += 11;
            r *= 25214903917;
        }
        once = true;
    }

    return random[x + y * 128];
}

uint8_t GB_camera_read_image(GB_gameboy_t *gb, uint16_t addr)
{
    uint8_t tile_x = addr / 0x10 % 0x10;
    uint8_t tile_y = addr / 0x10 / 0x10;

    uint8_t y = ((addr >> 1) & 0x7) + tile_y * 8;
    uint8_t bit = addr & 1;

    uint8_t ret = 0;

    for (uint8_t x = tile_x * 8; x < tile_x * 8 + 8; x++) {

        int color = gb->camera_get_pixel_callback? ~gb->camera_get_pixel_callback(gb, x,y) : (rand() & 0xFF);

        /* Dither using a deterministic random */
        color += dither_random(x, y);
        if (color > 255) {
            color = 255;
        }
        else if (color < 0) {
            color = 0;
        }

        ret <<= 1;
        ret |= (color >> (6 + bit)) & 1;
    }

    return ret;
}

void GB_set_camera_get_pixel_callback(GB_gameboy_t *gb, GB_camera_get_pixel_callback_t callback)
{
    gb->camera_get_pixel_callback = callback;
}

void GB_set_camera_update_request_callback(GB_gameboy_t *gb, GB_camera_update_request_callback_t callback)
{
    gb->camera_update_request_callback = callback;
}

void GB_camera_updated(GB_gameboy_t *gb)
{
    gb->camera_registers[0] = 0;
}

void GB_camera_write_register(GB_gameboy_t *gb, uint16_t addr, uint8_t value)
{
    addr &= 0x7F;
    if (addr == 0) {
        if ((value & 1) && gb->camera_update_request_callback) {
            /* If no callback is set, ignore the write as if the camera is instantly done */
            gb->camera_update_request_callback(gb);
            gb->camera_registers[0] = 1;
        }
    }
    else {
        if (addr >= 0x36) {
            GB_log(gb, "Wrote invalid camera register %02x: %2x\n", addr, value);
        }
        /* Todo: find out what these registers do */
        gb->camera_registers[addr] = value;
    }
}
uint8_t GB_camera_read_register(GB_gameboy_t *gb, uint16_t addr)
{
    if ((addr & 0x7F) == 0) {
        return gb->camera_registers[0];
    }
    return 0;
}