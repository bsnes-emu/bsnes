#ifndef MBC_h
#define MBC_h
#include "gb.h"

extern const GB_cartridge_t GB_cart_defs[256];
void GB_update_mbc_mappings(GB_gameboy_t *gb);
void GB_configure_cart(GB_gameboy_t *gb);
#endif /* MBC_h */
