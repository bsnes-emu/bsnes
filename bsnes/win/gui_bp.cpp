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

HWND hwndDBP;

void debug_refresh_bp(void) {
char str[64*16], t[64];
  if(debug_get_state() == DEBUGMODE_DISABLED)return;

  strcpy(str, "");
  for(int i=0;i<16;i++) {
    sprintf(t, "%0.2d: ", i);
    strcat(str, t);
    if(gx816->bp_list[i].flags == BP_OFF) {
      strcat(str, "------ ---- -- (Disabled)");
    } else {
      sprintf(t, "%0.6x %c%c%c%c ", gx816->bp_list[i].offset,
        (gx816->bp_list[i].flags & BP_READ )?'R':'r',
        (gx816->bp_list[i].flags & BP_WRITE)?'W':'w',
        (gx816->bp_list[i].flags & BP_EXEC )?'X':'x',
        (gx816->bp_list[i].flags & BP_VAL  )?'V':'v');
      strcat(str, t);
      if(gx816->bp_list[i].flags & BP_VAL) {
        sprintf(t, "%0.2x ", gx816->bp_list[i].value);
        strcat(str, t);
      } else strcat(str, "-- ");
      sprintf(t, "%10d", gx816->bp_list[i].hit_count);
      strcat(str, t);
    }
    if(i != 15)strcat(str, "\n");
  }

  SetDlgItemText(hwndDBP, DBP_LIST, str);
}

long __stdcall wndprocDBP(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
char str[256];
ulong num, offset, val, flags;
int i;
  if(msg == WM_DESTROY || msg == WM_CLOSE)return 0; //don't allow debugger to be closed (yet)

  if(msg == WM_COMMAND) {
    switch(LOWORD(wparam)) {
    case DBP_ADDBP:
      GetDlgItemText(hwndDBP, DBP_BPNUM, str, 255);
      num = strdec(str);
      GetDlgItemText(hwndDBP, DBP_BPOFFSET, str, 255);
      offset = strhex(str) & 0xffffff;
      GetDlgItemText(hwndDBP, DBP_BPVAL, str, 255);
      val = strhex(str);

      flags = 0;
      if(SendMessage(GetDlgItem(hwndDBP, DBP_BPR), BM_GETCHECK, 0, 0))flags |= BP_READ;
      if(SendMessage(GetDlgItem(hwndDBP, DBP_BPW), BM_GETCHECK, 0, 0))flags |= BP_WRITE;
      if(SendMessage(GetDlgItem(hwndDBP, DBP_BPX), BM_GETCHECK, 0, 0))flags |= BP_EXEC;
      if(SendMessage(GetDlgItem(hwndDBP, DBP_BPV), BM_GETCHECK, 0, 0))flags |= BP_VAL;

      if(num > 15)dprintf("Invalid breakpoint #: %d -- 0-15 valid", num);
      else {
        gx816->bp_list[num].offset = offset;
        gx816->bp_list[num].flags  = flags;
        if(gx816->bp_list[num].flags & BP_VAL)gx816->bp_list[num].value = val;
        else                                  gx816->bp_list[num].value = 0;
        gx816->bp_list[num].hit_count = 0;
      }
      debugger.refresh_bp = true;
      break;
    case DBP_REMBP:
      GetDlgItemText(hwndDBP, DBP_BPNUM, str, 255);
      num = strdec(str);
      if(num > 15)dprintf("Invalid breakpoint #: %d -- 0-15 valid", num);
      else {
        gx816->bp_list[num].offset    = 0;
        gx816->bp_list[num].flags     = BP_OFF;
        gx816->bp_list[num].value     = 0;
        gx816->bp_list[num].hit_count = 0;
      }
      debugger.refresh_bp = true;
      break;
    case DBP_CLRBP:
      for(i=0;i<16;i++) {
        gx816->bp_list[i].offset    = 0;
        gx816->bp_list[i].flags     = BP_OFF;
        gx816->bp_list[i].value     = 0;
        gx816->bp_list[i].hit_count = 0;
      }
      debugger.refresh_bp = true;
      break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void CreateDBP(void) {
  CreateWindowEx(WS_EX_STATICEDGE, "STATIC", "", WS_CHILD|WS_VISIBLE, 5, 5, 205, 228, hwndDBP, (HMENU)DBP_LIST, GetModuleHandle(0), 0);
  CreateWindow("STATIC", "BP #:  Offset:", WS_CHILD|WS_VISIBLE, 215, 5, 90, 15, hwndDBP, (HMENU)DBP_STATIC1, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "00", WS_CHILD|WS_VISIBLE,     215, 20, 30, 23, hwndDBP, (HMENU)DBP_BPNUM, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "000000", WS_CHILD|WS_VISIBLE, 245, 20, 60, 23, hwndDBP, (HMENU)DBP_BPOFFSET, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "R", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,    215,  44,  30, 18, hwndDBP, (HMENU)DBP_BPR,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "W", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,    245,  44,  30, 18, hwndDBP, (HMENU)DBP_BPW,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "X", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,    275,  44,  30, 18, hwndDBP, (HMENU)DBP_BPX,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "V", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,    215,  65,  30, 18, hwndDBP, (HMENU)DBP_BPV,   GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "00", WS_CHILD|WS_VISIBLE, 245,  62,  60, 23, hwndDBP, (HMENU)DBP_BPVAL, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Set BP",        WS_CHILD|WS_VISIBLE,        215,  88,  90, 20, hwndDBP, (HMENU)DBP_ADDBP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Clear BP",      WS_CHILD|WS_VISIBLE,        215, 108,  90, 20, hwndDBP, (HMENU)DBP_REMBP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Clear All BPs", WS_CHILD|WS_VISIBLE,        215, 128,  90, 20, hwndDBP, (HMENU)DBP_CLRBP, GetModuleHandle(0), 0);

  SendDlgItemMessage(hwndDBP, DBP_LIST,     WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_STATIC1,  WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPNUM,    WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPOFFSET, WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPR,      WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPW,      WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPX,      WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPV,      WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_BPVAL,    WM_SETFONT, (WPARAM)hFontFixed, TRUE);
  SendDlgItemMessage(hwndDBP, DBP_ADDBP,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDBP, DBP_REMBP,    WM_SETFONT, (WPARAM)hFont,      TRUE);
  SendDlgItemMessage(hwndDBP, DBP_CLRBP,    WM_SETFONT, (WPARAM)hFont,      TRUE);
}
