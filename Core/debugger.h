#ifndef debugger_h
#define debugger_h
#include "gb.h"

void GB_debugger_run(GB_gameboy_t *gb);
void GB_debugger_call_hook(GB_gameboy_t *gb);
void GB_debugger_ret_hook(GB_gameboy_t *gb);

#endif /* debugger_h */
