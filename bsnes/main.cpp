/* bsnes
   project started: 10/14/2004
   author: byuu */

#include "base.h"
#include "main.h"
#include "timing/timing.h"
#include "cpu/g65816.h"
#include "apu/spc700.h"
#include "bridge/bridge.h"
extern snes_timer  *snes_time;
extern g65816      *gx816;
extern sony_spc700 *spc700;
extern ppustate     ppu;
extern port_bridge *cpu_apu_bridge;
debugstate          debugger;

void debug_test_breakpoint_hit(byte source, byte flag, ulong offset, byte value, int i) {
  dprintf("* breakpoint %d hit", i);
  debugger.bp_list[i].hit_count++;
  debugger.refresh_bp = true;
  debug_set_state(DEBUGMODE_WAIT);
  if(source == BPSRC_SPCRAM) {
    disas_spc700_op();
  } else {
    disas_g65816_op();
  }
}

void debug_test_breakpoint(byte source, byte flag, ulong offset, byte value) {
int i;
  if(debug_get_state() == DEBUGMODE_DISABLED)return;

  for(i=0;i<16;i++) {
    if(debugger.bp_list[i].source != source)continue;
    if(debugger.bp_list[i].flags & flag) {
      if(debugger.bp_list[i].offset == offset) {
        if(debugger.bp_list[i].flags & BP_VAL) {
          if(debugger.bp_list[i].value == value) {
            debug_test_breakpoint_hit(source, flag, offset, value, i);
          }
        } else {
          debug_test_breakpoint_hit(source, flag, offset, value, i);
        }
      }
    }
  }
}

ulong debug_write_status(void) {
  if(debugger_enabled() == false)return DEBUGWRITE_NONE;

  if(debug_get_state() == DEBUGMODE_RUN) {
    if(debugger.trace_enabled == false)return DEBUGWRITE_NONE;
    if(debugger.trace_output_enabled == true) {
      return DEBUGWRITE_CONSOLE | DEBUGWRITE_TRACE;
    }
    return DEBUGWRITE_TRACE;
  }

  return DEBUGWRITE_CONSOLE;
}

void init_debugstate(void) {
  debugger.mode = DEBUGMODE_DISABLED;

  debugger.mem_ptr        = 0x7e0000;
  debugger.disas_cpu_op   = true;
  debugger.disas_apu_op   = true;
  debugger.refresh_mem    = true;
  debugger.refresh_bp     = true;
  debugger.refresh_status = true;

  debugger.cpu_op_executed = false;
  debugger.apu_op_executed = false;

  debugger.output_cpu_instrs    = true;
  debugger.output_apu_instrs    = true;
  debugger.output_debug_info    = true;
  debugger.trace_enabled        = false;
  debugger.trace_output_enabled = false;

  debugger.lock_up = debugger.lock_down = debugger.lock_left = debugger.lock_right = false;
  debugger.lock_a = debugger.lock_b = debugger.lock_x = debugger.lock_y = false;
  debugger.lock_l = debugger.lock_r = debugger.lock_select = debugger.lock_start = false;
}

void RunSNES(void) {
  if(emu_state.rom_loaded == false)return;

  if(debugger_enabled() == false) {
    gx816->Run();
    spc700->Run();
    return;
  }

  if(debug_get_state() == DEBUGMODE_RUN) {
    gx816->Run();
    spc700->Run();
    if(debugger.trace_enabled == true) {
      if(debugger.disas_cpu_op == true) {
        disas_g65816_op();
        debugger.disas_cpu_op = false;
      }
      if(debugger.disas_apu_op == true) {
        disas_spc700_op();
        debugger.disas_apu_op = false;
      }
    }
  } else {
    if(debugger.disas_cpu_op == true) {
      disas_g65816_op();
      debugger.disas_cpu_op = false;
    }
    if(debugger.disas_apu_op == true) {
      disas_spc700_op();
      debugger.disas_apu_op = false;
    }
    if(debugger.refresh_mem == true) {
      debug_refresh_mem();
      debugger.refresh_mem = false;
    }
    if(debugger.refresh_bp == true) {
      debug_refresh_bp();
      debugger.refresh_bp = false;
    }
    if(debugger.refresh_status == true) {
      debug_update_status();
      debugger.refresh_status = false;
    }
    if(debug_get_state() == DEBUGMODE_WAIT)return;

    if(debug_get_state() == DEBUGMODE_CPUSTEP) {
      gx816->Run();
      spc700->Run();
      if(debugger.cpu_op_executed == true) {
        debug_set_state(DEBUGMODE_WAIT);
      }
    }

    if(debug_get_state() == DEBUGMODE_APUSTEP) {
      gx816->Run();
      spc700->Run();
      if(debugger.apu_op_executed == true) {
        debug_set_state(DEBUGMODE_WAIT);
      }
    }
  }
  debug_update_status();
}

extern vfunc spc700_optbl[256];

void InitSNES(void) {
int i, z;
  snes_time = new snes_timer();
  gx816 = new g65816();
  gx816->PowerOn(1);
  spc700 = new sony_spc700();
  spc700->Reset();
  snes_time->reset_clock();
  cpu_apu_bridge = new port_bridge();
  if(*emu_state.rom_name == 0) {
    emu_state.rom_loaded = false;
  } else {
    gx816->LoadROM();
    emu_state.rom_loaded = true;
  }
  for(i=0;i<16;i++) {
    debugger.bp_list[i].offset    = 0;
    debugger.bp_list[i].flags     = BP_OFF;
    debugger.bp_list[i].source    = BPSRC_MEM;
    debugger.bp_list[i].value     = 0;
    debugger.bp_list[i].hit_count = 0;
  }
}

void ResetSNES(void) {
int i;
  gx816->Reset();
  spc700->Reset();
  snes_time->reset_clock();
  for(i=0;i<16;i++) {
    debugger.bp_list[i].offset    = 0;
    debugger.bp_list[i].flags     = BP_OFF;
    debugger.bp_list[i].source    = BPSRC_MEM;
    debugger.bp_list[i].value     = 0;
    debugger.bp_list[i].hit_count = 0;
  }
}

#include <windows.h>
int __stdcall WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
char fn[MAX_PATH];
  strcpy(fn, lpcmdline);
//remove quotes from filename, if neccesary (if path contains spaces, quotes will be around command line arg)
  if(*fn == '\"') {
    strcpy(emu_state.rom_name, fn + 1);
    emu_state.rom_name[strlen(emu_state.rom_name) - 1] = 0;
  } else {
    strcpy(emu_state.rom_name, fn);
  }
//create save ram file name
  strcpy(emu_state.sram_name, emu_state.rom_name);
  if(strlen(emu_state.sram_name) > 4) {
    emu_state.sram_name[strlen(emu_state.sram_name) - 4] = 0;
  }
  strcat(emu_state.sram_name, ".srm");

  init_debugstate();
  __winmain(); //located in win/gui.cpp

  return 0;
}
