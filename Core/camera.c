#include "camera.h"

uint8_t GB_camera_read_image(GB_gameboy_t *gb, uint16_t addr)
{
    if (gb->camera_registers[GB_CAMERA_FLAGS] & 1) {
        /* Forbid reading the image while the camera is busy. */
        return 0xFF;
    }
    uint8_t tile_x = addr / 0x10 % 0x10;
    uint8_t tile_y = addr / 0x10 / 0x10;

    uint8_t y = ((addr >> 1) & 0x7) + tile_y * 8;
    uint8_t bit = addr & 1;

    uint8_t ret = 0;

    for (uint8_t x = tile_x * 8; x < tile_x * 8 + 8; x++) {

        long color = gb->camera_get_pixel_callback? gb->camera_get_pixel_callback(gb, x,y) : (rand() & 0xFF);

        /* Color is multiplied by the multiplier register. */
        /* It is unknown what register 1 does, but changing bits 2-3 from 0x4 to 0x8 seems equivalent to adding 0x2000
           to the multiplier. Is it related to actual exposure time? */
        unsigned long multiplier_bias = (gb->camera_registers[GB_CAMERA_UNKNOWN_FLAGS] & 0xF) * 0x800;
        color = color * ((gb->camera_registers[GB_CAMERA_MULTIPLIER_HIGH] << 8) + gb->camera_registers[GB_CAMERA_MULTIPLIER_LOW] + multiplier_bias) / 0x3000;

        /* The camera's registers are used as a threshold pattern, which defines the dithering */
        uint8_t pattern_base = ((x & 3) + (y & 3) * 4) * 3 + GB_CAMERA_DITHERING_PATTERN_START;

        /* Todo: I have absolutely no reason to assume that this does not go backwards! */
        if (color < gb->camera_registers[pattern_base]) {
            color = 3;
        }
        else if (color < gb->camera_registers[pattern_base + 1]) {
            color = 2;
        }
        else if (color < gb->camera_registers[pattern_base + 2]) {
            color = 1;
        }
        else {
            color = 0;
        }

        ret <<= 1;
        ret |= (color >> bit) & 1;
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
    gb->camera_registers[GB_CAMERA_FLAGS] &= ~1;
}

void GB_camera_write_register(GB_gameboy_t *gb, uint16_t addr, uint8_t value)
{
    addr &= 0x7F;
    if (addr == GB_CAMERA_FLAGS) {
        if ((value & 1) && !(gb->camera_registers[GB_CAMERA_FLAGS] & 1) && gb->camera_update_request_callback) {
            /* If no callback is set, ignore the write as if the camera is instantly done */
            gb->camera_registers[GB_CAMERA_FLAGS] |= 1;
            gb->camera_update_request_callback(gb);
        }
    }
    else {
        if (addr >= 0x36) {
            GB_log(gb, "Wrote invalid camera register %02x: %2x\n", addr, value);
            return;
        }
        /* Todo: find out what these registers do */
        gb->camera_registers[addr] = value;
    }
}
uint8_t GB_camera_read_register(GB_gameboy_t *gb, uint16_t addr)
{
    if ((addr & 0x7F) == 0) {
        return gb->camera_registers[GB_CAMERA_FLAGS];
    }
    return 0;
}