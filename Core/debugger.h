#ifndef debugger_h
#define debugger_h
#include "gb.h"

void GB_debugger_run(GB_gameboy_t *gb);
void GB_debugger_call_hook(GB_gameboy_t *gb);
void GB_debugger_ret_hook(GB_gameboy_t *gb);
void GB_debugger_test_write_watchpoint(GB_gameboy_t *gb, uint16_t addr, uint8_t value);
void GB_debugger_test_read_watchpoint(GB_gameboy_t *gb, uint16_t addr);

#endif /* debugger_h */
