#define DMEM_VIEW        100
#define DMEM_EDITWRAM    101
#define DMEM_UP40        102
#define DMEM_DOWN40      103
#define DMEM_UP400       104
#define DMEM_DOWN400     105
#define DMEM_UP4000      106
#define DMEM_DOWN4000    107
#define DMEM_UP40000     108
#define DMEM_DOWN40000   109
#define DMEM_TOWRAM      110
#define DMEM_TOROM       111
#define DMEM_TOVRAM      112
#define DMEM_TOCGRAM     113
#define DMEM_TOOAM       114
#define DMEM_TOOFFSET    115
#define DMEM_EDITLOC     116
#define DMEM_EDITVAL     117
#define DMEM_STATIC1     118
#define DMEM_STATIC2     119

#define DMEMMODE_WRAM  0
#define DMEMMODE_VRAM  1
#define DMEMMODE_CGRAM 2
#define DMEMMODE_OAM   3
ulong dmem_mode = DMEMMODE_WRAM;

HWND hwndDMEM;

void debug_refresh_mem(void) {
char str[64*16], t[16];
ulong ptr;
int x, y;
  if(debug_get_state() == DEBUGMODE_DISABLED)return;

  ptr = debugger.mem_ptr;
  strcpy(str, "");

  if(dmem_mode == DMEMMODE_WRAM) {
    ptr &= 0xffffff;
    for(y=0;y<16;y++) {
      sprintf(t, "%0.6x: ", ptr);
      strcat(str, t);
      for(x=0;x<16;x++) {
        sprintf(t, "%0.2x", gx816->mem_read(MEMMODE_LONG, MEMSIZE_BYTE, ptr++, MEMACCESS_DEBUGGER));
        ptr &= 0xffffff;
        strcat(str, t);
        if(x != 15)strcat(str, " ");
      }
      if(y != 15)strcat(str, "\n");
    }
  } else if(dmem_mode == DMEMMODE_VRAM) {
    ptr &= 0xffff;
    for(y=0;y<16;y++) {
      sprintf(t, "--%0.4x: ", ptr);
      strcat(str, t);
      for(x=0;x<16;x++) {
        sprintf(t, "%0.2x", ppu.vram[ptr++]);
        ptr &= 0xffff;
        strcat(str, t);
        if(x != 15)strcat(str, " ");
      }
      if(y != 15)strcat(str, "\n");
    }
  } else if(dmem_mode == DMEMMODE_CGRAM) {
    ptr &= 0x01ff;
    for(y=0;y<16;y++) {
      sprintf(t, "---%0.3x: ", ptr);
      strcat(str, t);
      for(x=0;x<16;x++) {
        sprintf(t, "%0.2x", ppu.cgram[ptr++]);
        ptr &= 0x01ff;
        strcat(str, t);
        if(x != 15)strcat(str, " ");
      }
      if(y != 15)strcat(str, "\n");
    }
  } else if(dmem_mode == DMEMMODE_OAM) {
    ptr &= 0x03ff;
    for(y=0;y<16;y++) {
      sprintf(t, "---%0.3x: ", ptr);
      strcat(str, t);
      for(x=0;x<16;x++) {
        sprintf(t, "%0.2x", oam_read(ptr++));
        ptr &= 0x03ff;
        strcat(str, t);
        if(x != 15)strcat(str, " ");
      }
      if(y != 15)strcat(str, "\n");
    }
  }

  SetDlgItemText(hwndDMEM, DMEM_VIEW, str);
}

void __mask_mem_ptr(void) {
  if(dmem_mode == DMEMMODE_WRAM )debugger.mem_ptr &= 0xffffff;
  if(dmem_mode == DMEMMODE_VRAM )debugger.mem_ptr &= 0x00ffff;
  if(dmem_mode == DMEMMODE_CGRAM)debugger.mem_ptr &= 0x0001ff;
  if(dmem_mode == DMEMMODE_OAM  )debugger.mem_ptr &= 0x0003ff;
}

long __stdcall wndprocDMEM(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
char str[256];
ulong pos, val;
  if(msg == WM_DESTROY || msg == WM_CLOSE)return 0; //don't allow debugger to be closed (yet)

  if(msg == WM_COMMAND) {
    switch(LOWORD(wparam)) {
    case DMEM_EDITWRAM:
      GetDlgItemText(hwndDMEM, DMEM_EDITLOC, str, 255);
      pos = strhex(str);
      GetDlgItemText(hwndDMEM, DMEM_EDITVAL, str, 255);
      val = strhex(str);
      if(dmem_mode == DMEMMODE_WRAM) {
        gx816->mem_write(MEMMODE_LONG, MEMSIZE_BYTE, pos & 0xffffff, val, MEMACCESS_DEBUGGER);
      } else if(dmem_mode == DMEMMODE_VRAM) {
        ppu.vram[pos & 0xffff] = val;
      } else if(dmem_mode == DMEMMODE_CGRAM) {
        ppu.cgram[pos & 0x01ff] = val;
      } else if(dmem_mode == DMEMMODE_OAM) {
        oam_write(pos, val);
      }
      debug_refresh_mem();
      break;
    case DMEM_UP40:
      debugger.mem_ptr -= 0x40;
      debug_refresh_mem();
      __mask_mem_ptr();
      break;
    case DMEM_DOWN40:
      debugger.mem_ptr += 0x40;
      debug_refresh_mem();
      __mask_mem_ptr();
      break;
    case DMEM_UP400:
      debugger.mem_ptr -= 0x400;
      debug_refresh_mem();
      __mask_mem_ptr();
      break;
    case DMEM_DOWN400:
      debugger.mem_ptr += 0x400;
      debug_refresh_mem();
      __mask_mem_ptr();
      break;
    case DMEM_UP4000:
      debugger.mem_ptr -= 0x4000;
      debug_refresh_mem();
      __mask_mem_ptr();
      break;
    case DMEM_DOWN4000:
      debugger.mem_ptr += 0x4000;
      debug_refresh_mem();
      __mask_mem_ptr();
      break;
    case DMEM_UP40000:
      debugger.mem_ptr -= 0x40000;
      debug_refresh_mem();
      __mask_mem_ptr();
      break;
    case DMEM_DOWN40000:
      debugger.mem_ptr += 0x40000;
      debug_refresh_mem();
      __mask_mem_ptr();
      break;
    case DMEM_TOWRAM:
      dmem_mode = DMEMMODE_WRAM;
      debugger.mem_ptr = 0x7e0000;
      SetWindowText(hwndDMEM, "g65816 memory editor -- snes memory mode");
      debug_refresh_mem();
      break;
    case DMEM_TOROM:
      dmem_mode = DMEMMODE_WRAM;
      if(gx816->map == MEMMAP_LOROM)debugger.mem_ptr = 0x008000;
      else if(gx816->map == MEMMAP_HIROM)debugger.mem_ptr = 0xc00000;
      SetWindowText(hwndDMEM, "g65816 memory editor -- snes memory mode");
      debug_refresh_mem();
      break;
    case DMEM_TOVRAM:
      dmem_mode = DMEMMODE_VRAM;
      debugger.mem_ptr = 0x0000;
      SetWindowText(hwndDMEM, "g65816 memory editor -- ppu vram mode");
      debug_refresh_mem();
      break;
    case DMEM_TOCGRAM:
      dmem_mode = DMEMMODE_CGRAM;
      debugger.mem_ptr = 0x0000;
      SetWindowText(hwndDMEM, "g65816 memory editor -- ppu cgram mode");
      debug_refresh_mem();
      break;
    case DMEM_TOOAM:
      dmem_mode = DMEMMODE_OAM;
      debugger.mem_ptr = 0x0000;
      SetWindowText(hwndDMEM, "g65816 memory editor -- ppu oam mode");
      debug_refresh_mem();
      break;
    case DMEM_TOOFFSET:
      GetDlgItemText(hwndDMEM, DMEM_EDITLOC, str, 255);
      debugger.mem_ptr = strhex(str);
      __mask_mem_ptr();
      debug_refresh_mem();
      break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void CreateDMEM(void) {
  CreateWindowEx(WS_EX_STATICEDGE, "STATIC", "", WS_CHILD|WS_VISIBLE, 5, 5, 390, 228, hwndDMEM, (HMENU)DMEM_VIEW, GetModuleHandle(0), 0);
  CreateWindow("STATIC", "Offset:        Val:", WS_CHILD|WS_VISIBLE, 400, 5, 100, 15, hwndDMEM, (HMENU)DMEM_STATIC1, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE,  "EDIT", "000000", WS_CHILD|WS_VISIBLE, 400, 20, 60, 23, hwndDMEM, (HMENU)DMEM_EDITLOC, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE,  "EDIT", "00",     WS_CHILD|WS_VISIBLE, 460, 20, 30, 23, hwndDMEM, (HMENU)DMEM_EDITVAL, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Edit Memory", WS_CHILD|WS_VISIBLE, 400,  45,  90, 20, hwndDMEM, (HMENU)DMEM_EDITWRAM,  GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "-40",         WS_CHILD|WS_VISIBLE, 400,  70,  45, 20, hwndDMEM, (HMENU)DMEM_UP40,      GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "+40",         WS_CHILD|WS_VISIBLE, 445,  70,  45, 20, hwndDMEM, (HMENU)DMEM_DOWN40,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "-400",        WS_CHILD|WS_VISIBLE, 400,  90,  45, 20, hwndDMEM, (HMENU)DMEM_UP400,     GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "+400",        WS_CHILD|WS_VISIBLE, 445,  90,  45, 20, hwndDMEM, (HMENU)DMEM_DOWN400,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "-4000",       WS_CHILD|WS_VISIBLE, 400, 110,  45, 20, hwndDMEM, (HMENU)DMEM_UP4000,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "+4000",       WS_CHILD|WS_VISIBLE, 445, 110,  45, 20, hwndDMEM, (HMENU)DMEM_DOWN4000,  GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "-40000",      WS_CHILD|WS_VISIBLE, 400, 130,  45, 20, hwndDMEM, (HMENU)DMEM_UP40000,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "+40000",      WS_CHILD|WS_VISIBLE, 445, 130,  45, 20, hwndDMEM, (HMENU)DMEM_DOWN40000, GetModuleHandle(0), 0);
  CreateWindow("STATIC", "Seek To:",    WS_CHILD|WS_VISIBLE, 400, 155, 100, 15, hwndDMEM, (HMENU)DMEM_STATIC2,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "WRAM",        WS_CHILD|WS_VISIBLE, 400, 173,  45, 20, hwndDMEM, (HMENU)DMEM_TOWRAM,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "ROM",         WS_CHILD|WS_VISIBLE, 445, 173,  45, 20, hwndDMEM, (HMENU)DMEM_TOROM,     GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "VRAM",        WS_CHILD|WS_VISIBLE, 400, 193,  45, 20, hwndDMEM, (HMENU)DMEM_TOVRAM,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "CGRAM",       WS_CHILD|WS_VISIBLE, 445, 193,  45, 20, hwndDMEM, (HMENU)DMEM_TOCGRAM,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "OAM",         WS_CHILD|WS_VISIBLE, 400, 213,  45, 20, hwndDMEM, (HMENU)DMEM_TOOAM,     GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Offset",      WS_CHILD|WS_VISIBLE, 445, 213,  45, 20, hwndDMEM, (HMENU)DMEM_TOOFFSET,  GetModuleHandle(0), 0);

  SendDlgItemMessage(hwndDMEM, DMEM_VIEW,      WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_EDITLOC,   WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_EDITVAL,   WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_STATIC1,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_EDITWRAM,  WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_UP40,      WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_DOWN40,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_UP400,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_DOWN400,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_UP4000,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_DOWN4000,  WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_UP40000,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_DOWN40000, WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_STATIC2,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_TOWRAM,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_TOROM,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_TOVRAM,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_TOCGRAM,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_TOOAM,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDMEM, DMEM_TOOFFSET,  WM_SETFONT, (WPARAM)hFont,      TRUE);
}

