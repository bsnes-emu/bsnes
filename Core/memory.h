#ifndef memory_h
#define memory_h
#include "defs.h"
#include <stdint.h>

typedef uint8_t (*GB_read_memory_callback_t)(GB_gameboy_t *gb, uint16_t addr, uint8_t data);
void GB_set_read_memory_callback(GB_gameboy_t *gb, GB_read_memory_callback_t callback);

uint8_t GB_read_memory(GB_gameboy_t *gb, uint16_t addr);
void GB_write_memory(GB_gameboy_t *gb, uint16_t addr, uint8_t value);
#ifdef GB_INTERNAL
internal void GB_dma_run(GB_gameboy_t *gb);
internal void GB_hdma_run(GB_gameboy_t *gb);
internal void GB_trigger_oam_bug(GB_gameboy_t *gb, uint16_t address);
#endif

#endif /* memory_h */
