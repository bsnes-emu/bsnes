#include "camera.h"

uint8_t GB_camera_read_image(GB_gameboy_t *gb, uint16_t addr)
{
    if (gb->camera_registers[GB_CAMERA_SHOOT_AND_1D_FLAGS] & 1) {
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

        static const double gain_values[] = {0.8809390, 0.9149149, 0.9457498, 0.9739758,
                                            1.0000000, 1.0241412, 1.0466537, 1.0677433,
                                            1.0875793, 1.1240310, 1.1568911, 1.1868043,
                                            1.2142561, 1.2396208, 1.2743837, 1.3157323,
                                            1.3525190, 1.3856512, 1.4157897, 1.4434309,
                                            1.4689574, 1.4926697, 1.5148087, 1.5355703,
                                            1.5551159, 1.5735801, 1.5910762, 1.6077008,
                                            1.6235366, 1.6386550, 1.6531183, 1.6669808};
        /* Multiply color by gain value */
        color *= gain_values[gb->camera_registers[GB_CAMERA_GAIN_AND_EDGE_ENCHANCEMENT_FLAGS] & 0x1F];


        /* Color is multiplied by the exposure register to simulate exposure. */
        color = color * ((gb->camera_registers[GB_CAMERA_EXPOSURE_HIGH] << 8) + gb->camera_registers[GB_CAMERA_EXPOSURE_LOW]) / 0x1000;

        /* The camera's registers are used as a threshold pattern, which defines the dithering */
        uint8_t pattern_base = ((x & 3) + (y & 3) * 4) * 3 + GB_CAMERA_DITHERING_PATTERN_START;

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
    gb->camera_registers[GB_CAMERA_SHOOT_AND_1D_FLAGS] &= ~1;
}

void GB_camera_write_register(GB_gameboy_t *gb, uint16_t addr, uint8_t value)
{
    addr &= 0x7F;
    if (addr == GB_CAMERA_SHOOT_AND_1D_FLAGS) {
        value &= 0x7;
        if ((value & 1) && !(gb->camera_registers[GB_CAMERA_SHOOT_AND_1D_FLAGS] & 1) && gb->camera_update_request_callback) {
            /* If no callback is set, ignore the write as if the camera is instantly done */
            gb->camera_registers[GB_CAMERA_SHOOT_AND_1D_FLAGS] |= 1;
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
        return gb->camera_registers[GB_CAMERA_SHOOT_AND_1D_FLAGS];
    }
    return 0;
}