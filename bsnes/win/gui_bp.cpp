#define DBP_WIDTH  380
#define DBP_HEIGHT 243

#define DBP_LIST         100
#define DBP_ADDBP        101
#define DBP_REMBP        102
#define DBP_CLRBP        103
#define DBP_STATIC1      104
#define DBP_BPNUM        105
#define DBP_BPOFFSET     106
#define DBP_BPR          107
#define DBP_BPW          108
#define DBP_BPX          109
#define DBP_BPV          110
#define DBP_BPVAL        111
#define DBP_STATIC2      112
#define DBP_SRCMEM       113
#define DBP_SRCVRAM      114
#define DBP_SRCCGRAM     115
#define DBP_SRCOAM       116
#define DBP_SRCSPCRAM    117
#define DBP_SRCMODE      118

HWND hwndDBP;
byte debug_bp_src;

void debug_refresh_bp(void) {
char str[128*16], t[128];
  if(debugger_enabled() == false)return;

  strcpy(str, "");
  for(int i=0;i<16;i++) {
    sprintf(t, "%0.2d: ", i);
    strcat(str, t);
    if(debugger.bp_list[i].flags == BP_OFF) {
      strcat(str, "------ ---- -- ------ (Disabled)");
    } else {
      sprintf(t, "%0.6x %c%c%c%c ", debugger.bp_list[i].offset,
        (debugger.bp_list[i].flags & BP_READ )?'R':'r',
        (debugger.bp_list[i].flags & BP_WRITE)?'W':'w',
        (debugger.bp_list[i].flags & BP_EXEC )?'X':'x',
        (debugger.bp_list[i].flags & BP_VAL  )?'V':'v');
      strcat(str, t);
      if(debugger.bp_list[i].flags & BP_VAL) {
        sprintf(t, "%0.2x ", debugger.bp_list[i].value);
        strcat(str, t);
      } else strcat(str, "-- ");

      switch(debugger.bp_list[i].source) {
      case BPSRC_MEM:   strcat(str, "DRAM   ");break;
      case BPSRC_VRAM:  strcat(str, "VRAM   ");break;
      case BPSRC_CGRAM: strcat(str, "CGRAM  ");break;
      case BPSRC_OAM:   strcat(str, "OAM    ");break;
      case BPSRC_SPCRAM:strcat(str, "SPCRAM ");break;
      }

      sprintf(t, "%10d", debugger.bp_list[i].hit_count);
      strcat(str, t);
    }
    if(i != 15)strcat(str, "\r\n");
  }

  SetDlgItemText(hwndDBP, DBP_LIST, str);
}

long __stdcall wndprocDBP(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
char str[256];
ulong num, offset, val, flags;
int i, pos;
  if(msg == WM_DESTROY || msg == WM_CLOSE)return 0; //don't allow debugger to be closed (yet)

  if(msg == WM_COMMAND) {
    switch(LOWORD(wparam)) {
    case DBP_BPNUM:
      if(HIWORD(wparam) == CBN_SELCHANGE) {
        num = SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_GETCURSEL, 0, 0);
        sprintf(str, "%0.6x", debugger.bp_list[num].offset);
        SetDlgItemText(hwndDBP, DBP_BPOFFSET, str);
        sprintf(str, "%0.2x", debugger.bp_list[num].value);
        SetDlgItemText(hwndDBP, DBP_BPVAL, str);
        switch(debugger.bp_list[num].source) {
        case BPSRC_MEM:    pos = 0; break;
        case BPSRC_VRAM:   pos = 1; break;
        case BPSRC_CGRAM:  pos = 2; break;
        case BPSRC_OAM:    pos = 3; break;
        case BPSRC_SPCRAM: pos = 4; break;
        }
        SendDlgItemMessage(hwndDBP, DBP_SRCMODE, CB_SETCURSEL, (WPARAM)pos, 0);
        SendDlgItemMessage(hwndDBP, DBP_BPR, BM_SETCHECK, (debugger.bp_list[num].flags & BP_READ )?1:0, 0);
        SendDlgItemMessage(hwndDBP, DBP_BPW, BM_SETCHECK, (debugger.bp_list[num].flags & BP_WRITE)?1:0, 0);
        SendDlgItemMessage(hwndDBP, DBP_BPX, BM_SETCHECK, (debugger.bp_list[num].flags & BP_EXEC )?1:0, 0);
        SendDlgItemMessage(hwndDBP, DBP_BPV, BM_SETCHECK, (debugger.bp_list[num].flags & BP_VAL  )?1:0, 0);
      }
      break;
    case DBP_ADDBP:
      num = SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_GETCURSEL, 0, 0);
      GetDlgItemText(hwndDBP, DBP_BPOFFSET, str, 255);
      offset = strhex(str) & 0xffffff;
      GetDlgItemText(hwndDBP, DBP_BPVAL, str, 255);
      val = strhex(str);

      flags = 0;
      if(SendDlgItemMessage(hwndDBP, DBP_BPR, BM_GETCHECK, 0, 0))flags |= BP_READ;
      if(SendDlgItemMessage(hwndDBP, DBP_BPW, BM_GETCHECK, 0, 0))flags |= BP_WRITE;
      if(SendDlgItemMessage(hwndDBP, DBP_BPX, BM_GETCHECK, 0, 0))flags |= BP_EXEC;
      if(SendDlgItemMessage(hwndDBP, DBP_BPV, BM_GETCHECK, 0, 0))flags |= BP_VAL;

      debugger.bp_list[num].offset = offset;
      debugger.bp_list[num].flags  = flags;
      debugger.bp_list[num].source = debug_bp_src;
      if(debugger.bp_list[num].flags & BP_VAL) {
        debugger.bp_list[num].value = val;
      } else {
        debugger.bp_list[num].value = 0;
      }
      debugger.bp_list[num].hit_count = 0;
      debugger.refresh_bp = true;
      break;
    case DBP_REMBP:
      num = SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_GETCURSEL, 0, 0);
      debugger.bp_list[num].offset    = 0;
      debugger.bp_list[num].flags     = BP_OFF;
      debugger.bp_list[num].source    = BPSRC_MEM;
      debugger.bp_list[num].value     = 0;
      debugger.bp_list[num].hit_count = 0;
      debugger.refresh_bp = true;
      break;
    case DBP_CLRBP:
      for(i=0;i<16;i++) {
        debugger.bp_list[i].offset    = 0;
        debugger.bp_list[i].flags     = BP_OFF;
        debugger.bp_list[i].source    = BPSRC_MEM;
        debugger.bp_list[i].value     = 0;
        debugger.bp_list[i].hit_count = 0;
      }
      debugger.refresh_bp = true;
      break;
    case DBP_SRCMODE:
      if(HIWORD(wparam) == CBN_SELCHANGE) {
        pos = SendDlgItemMessage(hwndDBP, DBP_SRCMODE, CB_GETCURSEL, 0, 0);
        if(pos == 0) {
          debug_bp_src = BPSRC_MEM;
        } else if(pos == 1) {
          debug_bp_src = BPSRC_VRAM;
        } else if(pos == 2) {
          debug_bp_src = BPSRC_CGRAM;
        } else if(pos == 3) {
          debug_bp_src = BPSRC_OAM;
        } else if(pos == 4) {
          debug_bp_src = BPSRC_SPCRAM;
        }
      }
      break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void CreateDBP(void) {
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
    WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_READONLY,
    5, 5, 260, 233, hwndDBP, (HMENU)DBP_LIST, GetModuleHandle(0), 0);

  CreateWindow("STATIC", "BP #:      Offset:", WS_CHILD|WS_VISIBLE, 270, 5, 90, 15, hwndDBP, (HMENU)DBP_STATIC1, GetModuleHandle(0), 0);

  CreateWindow("COMBOBOX", "",
    WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_HASSTRINGS,
    270, 20, 45, 300, hwndDBP, (HMENU)DBP_BPNUM, GetModuleHandle(0), 0);
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"00");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"01");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"02");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"03");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"04");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"05");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"06");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"07");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"08");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"09");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"10");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"11");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"12");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"13");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"14");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_ADDSTRING, 0, (LPARAM)"15");
  SendDlgItemMessage(hwndDBP, DBP_BPNUM, CB_SETCURSEL, 0, 0);

  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "000000", WS_CHILD|WS_VISIBLE, 315, 20, 60, 23, hwndDBP, (HMENU)DBP_BPOFFSET, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "R", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,    270,  44,  30, 18, hwndDBP, (HMENU)DBP_BPR,     GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "W", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,    300,  44,  30, 18, hwndDBP, (HMENU)DBP_BPW,     GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "X", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,    330,  44,  30, 18, hwndDBP, (HMENU)DBP_BPX,     GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "V", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,    270,  65,  30, 18, hwndDBP, (HMENU)DBP_BPV,     GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "00", WS_CHILD|WS_VISIBLE, 300,  62,  30, 23, hwndDBP, (HMENU)DBP_BPVAL,   GetModuleHandle(0), 0);

  CreateWindow("STATIC", "Source:",       WS_CHILD|WS_VISIBLE,        270,  85,  90, 15, hwndDBP, (HMENU)DBP_STATIC2, GetModuleHandle(0), 0);
  CreateWindow("COMBOBOX", "",
    WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_HASSTRINGS,
    270, 100, 105, 200, hwndDBP, (HMENU)DBP_SRCMODE, GetModuleHandle(0), 0);
  SendDlgItemMessage(hwndDBP, DBP_SRCMODE, CB_ADDSTRING, 0, (LPARAM)"DRAM");
  SendDlgItemMessage(hwndDBP, DBP_SRCMODE, CB_ADDSTRING, 0, (LPARAM)"VRAM");
  SendDlgItemMessage(hwndDBP, DBP_SRCMODE, CB_ADDSTRING, 0, (LPARAM)"CGRAM");
  SendDlgItemMessage(hwndDBP, DBP_SRCMODE, CB_ADDSTRING, 0, (LPARAM)"OAM");
  SendDlgItemMessage(hwndDBP, DBP_SRCMODE, CB_ADDSTRING, 0, (LPARAM)"SPCRAM");
  SendDlgItemMessage(hwndDBP, DBP_SRCMODE, CB_SETCURSEL, 0, 0);

  CreateWindow("BUTTON", "Set BP",        WS_CHILD|WS_VISIBLE, 270, 125, 105, 20, hwndDBP, (HMENU)DBP_ADDBP,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Clear BP",      WS_CHILD|WS_VISIBLE, 270, 145, 105, 20, hwndDBP, (HMENU)DBP_REMBP,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Clear All BPs", WS_CHILD|WS_VISIBLE, 270, 165, 105, 20, hwndDBP, (HMENU)DBP_CLRBP,   GetModuleHandle(0), 0);

  SendDlgItemMessage(hwndDBP, DBP_LIST,      WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_STATIC1,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPNUM,     WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPOFFSET,  WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPR,       WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPW,       WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPX,       WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPV,       WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPVAL,     WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_SRCMODE,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDBP, DBP_STATIC2,   WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDBP, DBP_ADDBP,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDBP, DBP_REMBP,     WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDBP, DBP_CLRBP,     WM_SETFONT, (WPARAM)hFont,      TRUE);

  SendDlgItemMessage(hwndDBP, DBP_SRCMEM, BM_SETCHECK, 1, 0);
  debug_bp_src = BPSRC_MEM;
}
