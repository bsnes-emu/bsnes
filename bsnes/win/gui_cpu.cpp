#define DCPU_WIDTH  865
#define DCPU_HEIGHT 385

enum {
  DCPU_DISAS = 100,
  DCPU_CPUGROUP,
  DCPU_CPUSTEP,
  DCPU_CPUPROCEED,
  DCPU_CPUSKIP,
  DCPU_CPUTRACENUM,
  DCPU_CPUTRACE,
  DCPU_CPUDISABLE,
  DCPU_APUGROUP,
  DCPU_APUSTEP,
  DCPU_APUPROCEED,
  DCPU_APUSKIP,
  DCPU_APUTRACENUM,
  DCPU_APUTRACE,
  DCPU_APUDISABLE,
  DCPU_SYSGROUP,
  DCPU_SYSRUN,
  DCPU_SYSRUNTOFRAME,
  DCPU_SYSRUNTOVBLANK,
  DCPU_SYSRUNTONMI,
  DCPU_SYSRUNTOIRQ,
  DCPU_SYSRUNTOINT,
  DCPU_CFGGROUP,
  DCPU_CFGCPUOUT,
  DCPU_CFGAPUOUT,
  DCPU_CFGDBGOUT,
  DCPU_CFGTRACE,
  DCPU_CFGTRACEPRINT,
  DCPU_CFGREGTYPE,
  DCPU_CFGREGNUM,
  DCPU_CFGREGVAL,
  DCPU_CFGREGSET,
  DCPU_CFGLOCK,
  DCPU_CFGLOCKUP,
  DCPU_CFGLOCKDOWN,
  DCPU_CFGLOCKLEFT,
  DCPU_CFGLOCKRIGHT,
  DCPU_CFGLOCKA,
  DCPU_CFGLOCKB,
  DCPU_CFGLOCKX,
  DCPU_CFGLOCKY,
  DCPU_CFGLOCKL,
  DCPU_CFGLOCKR,
  DCPU_CFGLOCKSELECT,
  DCPU_CFGLOCKSTART,
  DCPU_STATUS
};

HWND hwndDCPU;
#define DEBUG_CONSOLE_LINES 250
char dcpu_disas_mem[DEBUG_CONSOLE_LINES][256];

void debug_update_console(void) {
int i;
char str[256 * DEBUG_CONSOLE_LINES];
  if(debug_write_status() == DEBUGWRITE_NONE)return;

  strcpy(str, "");
  for(i=0;i<DEBUG_CONSOLE_LINES;i++) {
    strcat(str, dcpu_disas_mem[i]);
    if(i != DEBUG_CONSOLE_LINES-1)strcat(str, "\r\n");
  }

  SetDlgItemText(hwndDCPU, DCPU_DISAS, str);
  SendDlgItemMessage(hwndDCPU, DCPU_DISAS, EM_SETSEL, 0, -1);
  SendDlgItemMessage(hwndDCPU, DCPU_DISAS, EM_SCROLLCARET, 0, 0);
}

void debug_update_status(void) {
char str[4096];
  if(!(debug_write_status() & DEBUGWRITE_CONSOLE))return;

  sprintf(str,
    "Scanline: %3d, HCycle Pos: %4d, HDot Pos: %3d, Master Cycle Pos: %10d\r\n"
    "NMI: %s, VIRQ: %s, HIRQ: %s, VIRQ Pos: %3d, HIRQ Pos: %3d\r\n"
    "CPU Status: { PC:%0.6x A:%0.4x X:%0.4x Y:%0.4x S:%0.4x D:%0.4x DB:%0.2x }\r\n"
    "APU Status: { PC:%0.4x A:%0.2x X:%0.2x Y:%0.2x SP:%0.2x YA:%0.4x }",
    snes_time->vscan_pos, snes_time->hcycle_pos, snes_time->hscan_pos, snes_time->master_cycles,
    (gx816->nmi_enabled   == true)?" Enabled":"Disabled",
    (ppu.vcounter_enabled == true)?" Enabled":"Disabled",
    (ppu.hcounter_enabled == true)?" Enabled":"Disabled",
    ppu.virq_pos, ppu.hirq_pos,
    gx816->regs.pc, gx816->regs.a.w, gx816->regs.x, gx816->regs.y,
    gx816->regs.s, gx816->regs.d, gx816->regs.db,
    spc700->regs.pc, spc700->regs.a, spc700->regs.x, spc700->regs.y,
    spc700->regs.sp, (spc700->regs.y << 8) | spc700->regs.a);
  SetDlgItemText(hwndDCPU, DCPU_STATUS, str);
}

void debug_write(ulong msg_type, char *s) {
int i;
  if(debugger_enabled() == false)return;

  switch(msg_type) {
  case DEBUGMSG_INFO:
    if(debugger.output_debug_info == false)return;
    break;
  case DEBUGMSG_CPU:
    if(debugger.output_cpu_instrs == false)return;
    break;
  case DEBUGMSG_APU:
    if(debugger.output_apu_instrs == false)return;
    break;
  }

  if(debugger.trace_enabled == true) {
    fprintf(debugger.trace_fp, "%s\r\n", s);
  }

  if(debug_write_status() & DEBUGWRITE_CONSOLE) {
    for(i=0;i<DEBUG_CONSOLE_LINES-1;i++)strcpy(dcpu_disas_mem[i], dcpu_disas_mem[i+1]);
    strcpy(dcpu_disas_mem[DEBUG_CONSOLE_LINES - 1], s);
    debug_update_console();
  }
}

void dprintf(char *s, ...) {
char str[4096];
va_list args;
int i;
  if(debugger_enabled() == false)return;

  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);

  debug_write(DEBUGMSG_INFO, str);
}

void dprintf(ulong msg_type, char *s, ...) {
char str[4096];
va_list args;
int i;
  if(debugger_enabled() == false)return;

  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);

  debug_write(msg_type, str);
}

void debug_set_state(byte state) {
  debugger.mode = state;
  if(hwndDCPU && state != DEBUGMODE_DISABLED) {
    if(state == DEBUGMODE_WAIT) {
      SetDlgItemText(hwndDCPU, DCPU_SYSRUN, "Run");
    } else if(state == DEBUGMODE_RUN) {
      SetDlgItemText(hwndDCPU, DCPU_SYSRUN, "Stop");
    }
  }
}

long __stdcall wndprocDCPU(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
FILE *fp;
int state;
  if(msg == WM_DESTROY || msg == WM_CLOSE)return 0; //don't allow debugger to be closed (yet)

  if(msg == WM_COMMAND) {
    switch(LOWORD(wparam)) {
    case DCPU_SYSRUN:
      if(debug_get_state() == DEBUGMODE_WAIT) {
        debug_set_state(DEBUGMODE_RUN);
      } else if(debug_get_state() == DEBUGMODE_RUN) {
        debug_set_state(DEBUGMODE_WAIT);
        debug_update_console();
        debug_update_status();
      }
      break;
    case DCPU_CPUSTEP:
      if(debug_get_state() == DEBUGMODE_WAIT) {
        debug_set_state(DEBUGMODE_CPUSTEP);
      }
      debugger.cpu_op_executed = false;
      break;
    case DCPU_APUSTEP:
      if(debug_get_state() == DEBUGMODE_WAIT) {
        debug_set_state(DEBUGMODE_APUSTEP);
      }
      debugger.apu_op_executed = false;
      break;
    case DCPU_CFGCPUOUT:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGCPUOUT, BM_GETCHECK, 0, 0);
      debugger.output_cpu_instrs = (state == 1)?true:false;
      break;
    case DCPU_CFGAPUOUT:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGAPUOUT, BM_GETCHECK, 0, 0);
      debugger.output_apu_instrs = (state == 1)?true:false;
      break;
    case DCPU_CFGDBGOUT:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGDBGOUT, BM_GETCHECK, 0, 0);
      debugger.output_debug_info = (state == 1)?true:false;
      break;
    case DCPU_CFGTRACE:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGTRACE, BM_GETCHECK, 0, 0);
      if(state == 0) {
        fclose(debugger.trace_fp);
        debugger.trace_enabled = false;
      } else {
        debugger.trace_fp = fopen("trace.log", "wb");
        debugger.trace_enabled = true;
      }
      break;
    case DCPU_CFGTRACEPRINT:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGTRACEPRINT, BM_GETCHECK, 0, 0);
      debugger.trace_output_enabled = (state == 1)?true:false;
      break;
    case DCPU_CFGLOCKUP:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKUP, BM_GETCHECK, 0, 0);
      debugger.lock_up   = (state == 1)?true:false;
      debugger.lock_down = false;
      SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKDOWN, BM_SETCHECK, 0, 0);
      break;
    case DCPU_CFGLOCKDOWN:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKDOWN, BM_GETCHECK, 0, 0);
      debugger.lock_down = (state == 1)?true:false;
      debugger.lock_up   = false;
      SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKUP, BM_SETCHECK, 0, 0);
      break;
    case DCPU_CFGLOCKLEFT:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKLEFT, BM_GETCHECK, 0, 0);
      debugger.lock_left  = (state == 1)?true:false;
      debugger.lock_right = false;
      SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKRIGHT, BM_SETCHECK, 0, 0);
      break;
    case DCPU_CFGLOCKRIGHT:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKRIGHT, BM_GETCHECK, 0, 0);
      debugger.lock_right = (state == 1)?true:false;
      debugger.lock_left  = false;
      SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKLEFT, BM_SETCHECK, 0, 0);
      break;
    case DCPU_CFGLOCKA:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKA, BM_GETCHECK, 0, 0);
      debugger.lock_a = (state == 1)?true:false;
      break;
    case DCPU_CFGLOCKB:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKB, BM_GETCHECK, 0, 0);
      debugger.lock_b = (state == 1)?true:false;
      break;
    case DCPU_CFGLOCKX:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKX, BM_GETCHECK, 0, 0);
      debugger.lock_x = (state == 1)?true:false;
      break;
    case DCPU_CFGLOCKY:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKY, BM_GETCHECK, 0, 0);
      debugger.lock_y = (state == 1)?true:false;
      break;
    case DCPU_CFGLOCKL:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKL, BM_GETCHECK, 0, 0);
      debugger.lock_l = (state == 1)?true:false;
      break;
    case DCPU_CFGLOCKR:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKR, BM_GETCHECK, 0, 0);
      debugger.lock_r = (state == 1)?true:false;
      break;
    case DCPU_CFGLOCKSELECT:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKSELECT, BM_GETCHECK, 0, 0);
      debugger.lock_select = (state == 1)?true:false;
      break;
    case DCPU_CFGLOCKSTART:
      state = SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKSTART, BM_GETCHECK, 0, 0);
      debugger.lock_start = (state == 1)?true:false;
      break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void CreateDCPU(void) {
int i;
  for(i=0;i<20;i++)strcpy(dcpu_disas_mem[i], "");
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
    WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY, 5, 5, 600, 290,
    hwndDCPU, (HMENU)DCPU_DISAS, GetModuleHandle(0), 0);

  CreateWindow("BUTTON", "cpu (wdc 65816)", WS_CHILD|WS_VISIBLE|BS_GROUPBOX, 610, 5, 250, 60, hwndDCPU, (HMENU)DCPU_CPUGROUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Step",    WS_CHILD|WS_VISIBLE, 615,  20, 80, 20, hwndDCPU, (HMENU)DCPU_CPUSTEP,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Proceed", WS_CHILD|WS_VISIBLE|WS_DISABLED, 695,  20, 80, 20, hwndDCPU, (HMENU)DCPU_CPUPROCEED, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Skip",    WS_CHILD|WS_VISIBLE|WS_DISABLED, 775,  20, 80, 20, hwndDCPU, (HMENU)DCPU_CPUSKIP,    GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0",
    WS_CHILD|WS_VISIBLE|ES_RIGHT|WS_DISABLED, 615,  40, 80, 20, hwndDCPU, (HMENU)DCPU_CPUTRACENUM, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Trace",   WS_CHILD|WS_VISIBLE|WS_DISABLED, 695,  40, 80, 20, hwndDCPU, (HMENU)DCPU_CPUTRACE,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Disable", WS_CHILD|WS_VISIBLE|WS_DISABLED, 775,  40, 80, 20, hwndDCPU, (HMENU)DCPU_CPUDISABLE, GetModuleHandle(0), 0);

  CreateWindow("BUTTON", "apu (sony spc700)", WS_CHILD|WS_VISIBLE|BS_GROUPBOX, 610, 70, 250, 60, hwndDCPU, (HMENU)DCPU_APUGROUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Step",    WS_CHILD|WS_VISIBLE, 615,  85, 80, 20, hwndDCPU, (HMENU)DCPU_APUSTEP,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Proceed", WS_CHILD|WS_VISIBLE|WS_DISABLED, 695,  85, 80, 20, hwndDCPU, (HMENU)DCPU_APUPROCEED, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Skip",    WS_CHILD|WS_VISIBLE|WS_DISABLED, 775,  85, 80, 20, hwndDCPU, (HMENU)DCPU_APUSKIP,    GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0",
    WS_CHILD|WS_VISIBLE|ES_RIGHT|WS_DISABLED, 615, 105, 80, 20, hwndDCPU, (HMENU)DCPU_APUTRACENUM, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Trace",   WS_CHILD|WS_VISIBLE|WS_DISABLED, 695, 105, 80, 20, hwndDCPU, (HMENU)DCPU_APUTRACE,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Disable", WS_CHILD|WS_VISIBLE|WS_DISABLED, 775, 105, 80, 20, hwndDCPU, (HMENU)DCPU_APUDISABLE, GetModuleHandle(0), 0);

  CreateWindow("BUTTON", "System",       WS_CHILD|WS_VISIBLE|BS_GROUPBOX, 610, 135, 250, 60, hwndDCPU, (HMENU)DCPU_SYSGROUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Run",          WS_CHILD|WS_VISIBLE, 615, 150, 80, 20, hwndDCPU, (HMENU)DCPU_SYSRUN,         GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "To Frame",     WS_CHILD|WS_VISIBLE|WS_DISABLED, 695, 150, 80, 20, hwndDCPU, (HMENU)DCPU_SYSRUNTOFRAME,  GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "To VBlank",    WS_CHILD|WS_VISIBLE|WS_DISABLED, 775, 150, 80, 20, hwndDCPU, (HMENU)DCPU_SYSRUNTOVBLANK, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "To NMI",       WS_CHILD|WS_VISIBLE|WS_DISABLED, 615, 170, 80, 20, hwndDCPU, (HMENU)DCPU_SYSRUNTONMI,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "To IRQ",       WS_CHILD|WS_VISIBLE|WS_DISABLED, 695, 170, 80, 20, hwndDCPU, (HMENU)DCPU_SYSRUNTOIRQ,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "To Interrupt", WS_CHILD|WS_VISIBLE|WS_DISABLED, 775, 170, 80, 20, hwndDCPU, (HMENU)DCPU_SYSRUNTOINT,    GetModuleHandle(0), 0);

  CreateWindow("BUTTON", "Settings",  WS_CHILD|WS_VISIBLE|BS_GROUPBOX, 610, 200, 250, 180, hwndDCPU, (HMENU)DCPU_CFGGROUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Output CPU instructions to console", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    615, 215, 240, 15, hwndDCPU, (HMENU)DCPU_CFGCPUOUT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Output APU instructions to console", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    615, 230, 240, 15, hwndDCPU, (HMENU)DCPU_CFGAPUOUT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Output debug info to console", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    615, 245, 240, 15, hwndDCPU, (HMENU)DCPU_CFGDBGOUT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Trace output to file (very slow)", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    615, 260, 240, 15, hwndDCPU, (HMENU)DCPU_CFGTRACE, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Print debug info while tracing (very slow)", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    615, 275, 240, 15, hwndDCPU, (HMENU)DCPU_CFGTRACEPRINT, GetModuleHandle(0), 0);
  CreateWindow("COMBOBOX", "", WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_HASSTRINGS|WS_DISABLED,
    615, 290,  60, 200, hwndDCPU, (HMENU)DCPU_CFGREGTYPE, GetModuleHandle(0), 0);
  CreateWindow("COMBOBOX", "", WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_HASSTRINGS|WS_DISABLED,
    675, 290,  60, 200, hwndDCPU, (HMENU)DCPU_CFGREGNUM, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "000000", WS_CHILD|WS_VISIBLE|ES_RIGHT|WS_DISABLED,
    735, 290,  60, 20, hwndDCPU, (HMENU)DCPU_CFGREGVAL, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Set Reg", WS_CHILD|WS_VISIBLE|WS_DISABLED,
    795, 290,  60, 20, hwndDCPU, (HMENU)DCPU_CFGREGSET, GetModuleHandle(0), 0);
  CreateWindow("STATIC", "Lock Joypad Buttons Down:", WS_CHILD|WS_VISIBLE, 615, 315, 240, 15, hwndDCPU, (HMENU)DCPU_CFGLOCK, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Up", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    615, 330, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Down", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    675, 330, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKDOWN, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Left", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    735, 330, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKLEFT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Right", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    795, 330, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKRIGHT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "A", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    615, 345, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKA, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "B", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    675, 345, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKB, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "X", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    735, 345, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKX, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Y", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    795, 345, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKY, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "L", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    615, 360, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKL, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "R", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    675, 360, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKR, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Select", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    735, 360, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKSELECT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Start", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,
    795, 360, 60, 15, hwndDCPU, (HMENU)DCPU_CFGLOCKSTART, GetModuleHandle(0), 0);

  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
    WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 5, 300, 600, 80,
    hwndDCPU, (HMENU)DCPU_STATUS, GetModuleHandle(0), 0);

  SendDlgItemMessage(hwndDCPU, DCPU_DISAS,          WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CPUGROUP,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CPUSTEP,        WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CPUPROCEED,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CPUSKIP,        WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CPUTRACENUM,    WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CPUTRACE,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CPUDISABLE,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_APUGROUP,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_APUSTEP,        WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_APUPROCEED,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_APUSKIP,        WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_APUTRACENUM,    WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_APUTRACE,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_APUDISABLE,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_SYSGROUP,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_SYSRUN,         WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_SYSRUNTOFRAME,  WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_SYSRUNTOVBLANK, WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_SYSRUNTONMI,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_SYSRUNTOIRQ,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_SYSRUNTOINT,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGGROUP,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGCPUOUT,      WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGAPUOUT,      WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGDBGOUT,      WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGTRACE,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGTRACEPRINT,  WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGREGTYPE,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGREGNUM,      WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGREGVAL,      WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGREGSET,      WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCK,        WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKUP,      WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKDOWN,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKLEFT,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKRIGHT,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKA,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKB,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKX,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKY,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKL,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKR,       WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKSELECT,  WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGLOCKSTART,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_STATUS,         WM_SETFONT, (WPARAM)hFontFixed, TRUE);

  SendDlgItemMessage(hwndDCPU, DCPU_CFGCPUOUT,     BM_SETCHECK, (WPARAM)(debugger.output_cpu_instrs    == true)?1:0, 0);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGAPUOUT,     BM_SETCHECK, (WPARAM)(debugger.output_apu_instrs    == true)?1:0, 0);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGDBGOUT,     BM_SETCHECK, (WPARAM)(debugger.output_debug_info    == true)?1:0, 0);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGTRACE,      BM_SETCHECK, (WPARAM)(debugger.trace_enabled        == true)?1:0, 0);
  SendDlgItemMessage(hwndDCPU, DCPU_CFGTRACEPRINT, BM_SETCHECK, (WPARAM)(debugger.trace_output_enabled == true)?1:0, 0);
}

