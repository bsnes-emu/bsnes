#ifndef camera_h
#define camera_h
#include "gb.h"

uint8_t GB_camera_read_image(GB_gameboy_t *gb, uint16_t addr);

void GB_set_camera_get_pixel_callback(GB_gameboy_t *gb, GB_camera_get_pixel_callback_t callback);
void GB_set_camera_update_request_callback(GB_gameboy_t *gb, GB_camera_update_request_callback_t callback);

void GB_camera_updated(GB_gameboy_t *gb);

void GB_camera_write_register(GB_gameboy_t *gb, uint16_t addr, uint8_t value);
uint8_t GB_camera_read_register(GB_gameboy_t *gb, uint16_t addr);

#endif
