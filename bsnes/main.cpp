/* bsnes
   project started 10/14/2004
   author byuu */

#include "base.h"
#include "main.h"
#include "timing/timing.h"
#include "cpu/g65816.h"
extern snes_timer *snes_time;
extern g65816 *gx816;
extern ppustate ppu;

vfunc RunSNES;

void RunSNES_NoDebug(void) {
byte l = 64;
  while(l--) {
    gx816->Run();
    gx816->Run();
    gx816->Run();
    gx816->Run();
    gx816->Run();
    gx816->Run();
    gx816->Run();
    gx816->Run();
  }
}

void RunSNES_Debug(void) {
  if(debug_get_state() == DEBUGMODE_NOROM)return;

  if(debug_get_state() == DEBUGMODE_RUN) {
    gx816->Run();
    if(debugger.trace_enabled == true) {
      disas_g65816_op();
    }
  } else {
    if(debugger.disas_op == true) {
      disas_g65816_op();
      debugger.disas_op = false;
    }
    if(debugger.refresh_mem == true) {
      debug_refresh_mem();
      debugger.refresh_mem = false;
    }
    if(debugger.refresh_bp == true) {
      debug_refresh_bp();
      debugger.refresh_bp = false;
    }
    if(debug_get_state() == DEBUGMODE_WAIT)return;

    if(debug_get_state() == DEBUGMODE_STEP) {
      gx816->Run();
      disas_g65816_op();
      debug_set_state(DEBUGMODE_WAIT);
    }
  }
}

void InitSNES(void) {
  snes_time = new snes_timer();
  gx816 = new g65816();
  gx816->PowerOn(1);
  if(*emu_state.rom_name == 0) {
    debug_set_state(DEBUGMODE_NOROM);
  } else {
    gx816->LoadROM();
  }

  if(debug_get_state() == DEBUGMODE_DISABLED) {
    RunSNES = RunSNES_NoDebug;
  } else {
    RunSNES = RunSNES_Debug;
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
//located in win/gui.cpp
  __winmain();
  return 0;
}
