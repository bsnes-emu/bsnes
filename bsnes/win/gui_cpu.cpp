#define DCPU_WIDTH  675
#define DCPU_HEIGHT 295

#define DCPU_DISAS       100
#define DCPU_RUN         101
#define DCPU_STEP        102
#define DCPU_PROCEED     103
#define DCPU_INFO        104
#define DCPU_TRACE       105
#define DCPU_CYCLES      106

HWND hwndDCPU;

char dcpu_disas_mem[20][256];

void dprintf(char *s, ...) {
char str[256*20];
va_list args;
int i;
  if(debug_get_state() == DEBUGMODE_DISABLED)return;

  va_start(args, s);
  vsprintf(str, s, args);
  va_end(args);

  if(debugger.trace_enabled == true) {
    fprintf(debugger.trace_fp, "%s\r\n", str);
  }

//if(debug_get_state() != DEBUGMODE_RUN) {
    for(i=0;i<19;i++)strcpy(dcpu_disas_mem[i], dcpu_disas_mem[i+1]);
    strcpy(dcpu_disas_mem[19], str);

    strcpy(str, "");
    for(i=0;i<20;i++) {
      strcat(str, dcpu_disas_mem[i]);
      if(i != 20)strcat(str, "\n");
    }

    SetDlgItemText(hwndDCPU, DCPU_DISAS, str);
//}
}

void debug_update_cycles(void) {
char str[256];
  if(debug_get_state() == DEBUGMODE_DISABLED ||
     debug_get_state() == DEBUGMODE_RUN)return;

  sprintf(str, "Scanline: %d\nCC: %d", ppu.vline_pos, snes_time->master_cycles);
  SetDlgItemText(hwndDCPU, DCPU_CYCLES, str);
}

void debug_set_state(byte state) {
  debugger.mode = state;
  if(hwndDCPU && state != DEBUGMODE_DISABLED) {
    if(state == DEBUGMODE_WAIT || state == DEBUGMODE_STEP) {
      SetDlgItemText(hwndDCPU, DCPU_RUN, "Run");
    } else {
      SetDlgItemText(hwndDCPU, DCPU_RUN, "Stop");
    }
  }

  if(state == DEBUGMODE_DISABLED) {
    RunSNES = RunSNES_NoDebug;
  } else {
    RunSNES = RunSNES_Debug;
  }
}

long __stdcall wndprocDCPU(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
FILE *fp;
  if(msg == WM_DESTROY || msg == WM_CLOSE)return 0; //don't allow debugger to be closed (yet)

  if(msg == WM_COMMAND) {
    switch(LOWORD(wparam)) {
    case DCPU_RUN:
      if(debug_get_state() != DEBUGMODE_RUN) {
        debug_set_state(DEBUGMODE_RUN);
        SetDlgItemText(hwndDCPU, DCPU_RUN, "Stop");
      } else {
        debug_set_state(DEBUGMODE_WAIT);
        disas_g65816_op();
        SetDlgItemText(hwndDCPU, DCPU_RUN, "Run");
      }
      break;
    case DCPU_STEP:
      if(debug_get_state() == DEBUGMODE_WAIT) {
        debug_set_state(DEBUGMODE_STEP);
      }
      break;
    case DCPU_PROCEED:
      fp = fopen("data.bin", "wb");
      fwrite(ppu.vram, 0x10000, 1, fp);
      fwrite(gx816->wram, 0x20000, 1, fp);
      fwrite(ppu.cgram, 512, 1, fp);
      fwrite(ppu.oam, 544, 1, fp);
      fclose(fp);
      break;
    case DCPU_INFO:
      fp = fopen("data.txt", "w");
      fprintf(fp,
              "Memory speed: %s\n"
              "Mode: %d\n"
              "BGs On: %d%d%d%d%d\n"
              "Sub BGs On: %d%d%d%d%d\n"
              "Tilemap Loc: %0.4x,%0.4x,%0.4x,%0.4x\n"
              "Tiledata Loc: %0.4x,%0.4x,%0.4x,%0.4x,%0.4x\n"
              "Tilemap Size: %d,%d,%d,%d\n"
              "Tile Size: %d,%d,%d,%d\n"
              "BG Priority Mode: %d\n"
              "OAM Base Size: %d\nOAM Name Sel: %d\nBG Enabled: %d\nBG Brightness: %d\n"
              "Window 1 Left: %d\nWindow 1 Right: %d\nWindow 2 Left: %d\nWindow 2 Right: %d\n"
              "Window 1 Enabled: %d%d%d%d%d\nWindow 2 Enabled: %d%d%d%d%d\n"
              "Window 1 Clipmode: %s,%s,%s,%s,%s\nWindow 2 Clipmode: %s,%s,%s,%s,%s\n"
              "BG Windowing Enabled: %d%d%d%d%d\nSub BG Windowing Enabled: %d%d%d%d%d\n"
              "BG Window Mask: %d,%d,%d,%d,%d\n"
              "Color Windows Enabled: %d%d\nColor Window Clipmodes: %d,%d\n"
              "Color Window Mask: %d\nColor Mask: %d\nSub Color Mask: %d\nAddsub Mode: %d\nColor Mode: %d\nColor Halve: %d\n"
              "BG Color Enabled: %d%d%d%d%d%d\n"
              "RGB Color Settings: %d,%d,%d\nVIRQ Counter Enabled: %s\nHIRQ Counter Enabled: %s\n"
              "VIRQ Pos: %d\nHIRQ Pos: %d\n"
              "Mode 7 Screen Flip: x: %s, y: %s\n"
              "m7a: %0.4x (%f) m7b: %0.4x (%f) m7c: %0.4x (%f) m7d: %0.4x (%f)\n"
              "m7x: %0.4x m7y: %0.4x\n",
        (gx816->memory_speed == 1)?"FastROM":"SlowROM", ppu.bg_mode,
        (ppu.bg_enabled[0] == true)?1:0, (ppu.bg_enabled[1] == true)?1:0,
        (ppu.bg_enabled[2] == true)?1:0, (ppu.bg_enabled[3] == true)?1:0, (ppu.bg_enabled[4] == true)?1:0,
        (ppu.ss_bg_enabled[0] == true)?1:0, (ppu.ss_bg_enabled[1] == true)?1:0,
        (ppu.ss_bg_enabled[2] == true)?1:0, (ppu.ss_bg_enabled[3] == true)?1:0, (ppu.ss_bg_enabled[4] == true)?1:0,
        ppu.bg_tilemap_loc[0], ppu.bg_tilemap_loc[1], ppu.bg_tilemap_loc[2], ppu.bg_tilemap_loc[3],
        ppu.bg_tiledata_loc[0], ppu.bg_tiledata_loc[1], ppu.bg_tiledata_loc[2], ppu.bg_tiledata_loc[3], ppu.oam_tiledata_loc,
        ppu.bg_tilemap_size[0], ppu.bg_tilemap_size[1], ppu.bg_tilemap_size[2], ppu.bg_tilemap_size[3],
        ppu.bg_tile_size[0], ppu.bg_tile_size[1], ppu.bg_tile_size[2], ppu.bg_tile_size[3], ppu.bg_priority_mode,
        ppu.oam_base_size, ppu.oam_name_sel, (ppu.display_disable == true)?0:1, ppu.display_brightness,
        ppu.window1_left, ppu.window1_right, ppu.window2_left, ppu.window2_right,
        (ppu.bg_window1_enabled[BG1] == true)?1:0, (ppu.bg_window1_enabled[BG2] == true)?1:0,
        (ppu.bg_window1_enabled[BG3] == true)?1:0, (ppu.bg_window1_enabled[BG4] == true)?1:0,
        (ppu.bg_window1_enabled[OAM] == true)?1:0,
        (ppu.bg_window2_enabled[BG1] == true)?1:0, (ppu.bg_window2_enabled[BG2] == true)?1:0,
        (ppu.bg_window2_enabled[BG3] == true)?1:0, (ppu.bg_window2_enabled[BG4] == true)?1:0,
        (ppu.bg_window2_enabled[OAM] == true)?1:0,
        (ppu.bg_window1_clipmode[BG1] == 1)?"in":"out", (ppu.bg_window1_clipmode[BG2] == 1)?"in":"out",
        (ppu.bg_window1_clipmode[BG3] == 1)?"in":"out", (ppu.bg_window1_clipmode[BG4] == 1)?"in":"out",
        (ppu.bg_window1_clipmode[OAM] == 1)?"in":"out",
        (ppu.bg_window2_clipmode[BG1] == 1)?"in":"out", (ppu.bg_window2_clipmode[BG2] == 1)?"in":"out",
        (ppu.bg_window2_clipmode[BG3] == 1)?"in":"out", (ppu.bg_window2_clipmode[BG4] == 1)?"in":"out",
        (ppu.bg_window2_clipmode[BG4] == 1)?"in":"out",
        (ppu.bg_windowing_enabled[BG1] == true)?1:0, (ppu.bg_windowing_enabled[BG2] == true)?1:0,
        (ppu.bg_windowing_enabled[BG3] == true)?1:0, (ppu.bg_windowing_enabled[BG4] == true)?1:0,
        (ppu.bg_windowing_enabled[OAM] == true)?1:0,
        (ppu.ss_bg_windowing_enabled[BG1] == true)?1:0, (ppu.ss_bg_windowing_enabled[BG2] == true)?1:0,
        (ppu.ss_bg_windowing_enabled[BG3] == true)?1:0, (ppu.ss_bg_windowing_enabled[BG4] == true)?1:0,
        (ppu.ss_bg_windowing_enabled[OAM] == true)?1:0,
        ppu.bg_window_mask[BG1], ppu.bg_window_mask[BG2],
        ppu.bg_window_mask[BG3], ppu.bg_window_mask[BG4], ppu.bg_window_mask[OAM],
        (ppu.color_window1_enabled == true)?1:0, (ppu.color_window2_enabled == true)?1:0,
        ppu.color_window1_clipmode, ppu.color_window2_clipmode, ppu.color_window_mask,
        ppu.color_mask, ppu.ss_color_mask, ppu.addsub_mode, ppu.color_mode, ppu.color_halve,
        (ppu.bg_color_enabled[BG1] == true)?1:0, (ppu.bg_color_enabled[BG2]  == true)?1:0,
        (ppu.bg_color_enabled[BG3] == true)?1:0, (ppu.bg_color_enabled[BG4]  == true)?1:0,
        (ppu.bg_color_enabled[OAM] == true)?1:0, (ppu.bg_color_enabled[BACK] == true)?1:0,
        ppu.color_r, ppu.color_g, ppu.color_b,
        (ppu.vcounter_enabled == true)?"Yes":"No", (ppu.hcounter_enabled == true)?"Yes":"No",
        ppu.virq_pos, ppu.hirq_pos,
        (ppu.mode7_vflip == true)?"Yes":"No", (ppu.mode7_hflip == true)?"Yes":"No",
        ppu.m7a, (float)ppu.m7a / 256, ppu.m7b, (float)ppu.m7b / 256,
        ppu.m7c, (float)ppu.m7c / 256, ppu.m7d, (float)ppu.m7d / 256,
        ppu.m7x, ppu.m7y
      );
      fclose(fp);
      break;
    case DCPU_TRACE:
      if(debugger.trace_enabled == true) {
        fclose(debugger.trace_fp);
        debugger.trace_enabled = false;
      } else {
        debugger.trace_fp = fopen("trace.log", "wb");
        debugger.trace_enabled = true;
      }
      break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void CreateDCPU(void) {
int i;
  for(i=0;i<20;i++)strcpy(dcpu_disas_mem[i], "");
  CreateWindowEx(WS_EX_STATICEDGE, "STATIC", "", WS_CHILD|WS_VISIBLE,     5,   5, 580, 285, hwndDCPU, (HMENU)DCPU_DISAS, GetModuleHandle(0), 0);

  CreateWindow("BUTTON", "Run",                   WS_CHILD|WS_VISIBLE,  590,   5,  80,  20, hwndDCPU, (HMENU)DCPU_RUN,     GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Step",                  WS_CHILD|WS_VISIBLE,  590,  25,  80,  20, hwndDCPU, (HMENU)DCPU_STEP,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Export Mem",            WS_CHILD|WS_VISIBLE,  590,  45,  80,  20, hwndDCPU, (HMENU)DCPU_PROCEED, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Info",                  WS_CHILD|WS_VISIBLE,  590,  65,  80,  20, hwndDCPU, (HMENU)DCPU_INFO,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Trace", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,  590,  85,  80,  20, hwndDCPU, (HMENU)DCPU_TRACE,   GetModuleHandle(0), 0);

  CreateWindow("STATIC", "Scanline: 0\nCycles: 0", WS_CHILD|WS_VISIBLE, 590, 260,  80,  30, hwndDCPU, (HMENU)DCPU_CYCLES,  GetModuleHandle(0), 0);

  SendDlgItemMessage(hwndDCPU, DCPU_DISAS,   WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_RUN,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_STEP,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_PROCEED, WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_INFO,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_TRACE,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDCPU, DCPU_CYCLES,  WM_SETFONT, (WPARAM)hFont,      TRUE);
}

