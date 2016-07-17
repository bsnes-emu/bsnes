#ifndef debugger_h
#define debugger_h
#include "gb.h"

void GB_debugger_run(GB_gameboy_t *gb);
void GB_debugger_handle_async_commands(GB_gameboy_t *gb);
void GB_debugger_call_hook(GB_gameboy_t *gb);
void GB_debugger_ret_hook(GB_gameboy_t *gb);
void GB_debugger_test_write_watchpoint(GB_gameboy_t *gb, uint16_t addr, uint8_t value);
void GB_debugger_test_read_watchpoint(GB_gameboy_t *gb, uint16_t addr);
void GB_debugger_load_symbol_file(GB_gameboy_t *gb, const char *path);
const GB_bank_symbol_t *GB_debugger_find_symbol(GB_gameboy_t *gb, uint16_t addr);
const char *GB_debugger_name_for_address(GB_gameboy_t *gb, uint16_t addr);
#endif /* debugger_h */
