bool BreakpointEditor::hit() {
//breakpoints are disabled when BP window is closed
  if(visible == false)return false;

  if(bp_hit == true) {
    bp_hit = false;
    return true;
  }
  return false;
}

void BreakpointEditor::test(uint32 message, uint32 addr, uint32 value) {
int i;
  if(visible == false)return;
  if(bp_enabled == false)return;

  for(i=0;i<16;i++) {
    if(bp[i].set == false)continue;

    switch(message) {
    case SNES::CPU_EXEC_OPCODE_END:
      if(!(bp[i].flags & FLAG_X))continue;
      if(bp[i].source != DRAM)continue;
      if(bp[i].addr != addr)continue;
      dprintf("* Breakpoint %d hit (CPU exec)", i);
      bp[i].hit_count++;
      bp_hit = true;
      w_bp->refresh();
      break;
    case SNES::APU_EXEC_OPCODE_END:
      if(!(bp[i].flags & FLAG_X))continue;
      if(bp[i].source != SPCRAM)continue;
      if(bp[i].addr != addr)continue;
      dprintf("* Breakpoint %d hit (APU exec)", i);
      bp[i].hit_count++;
      bp_hit = true;
      w_bp->refresh();
      break;
    case SNES::MEM_READ:
    case SNES::SPCRAM_READ:
    case SNES::VRAM_READ:
    case SNES::OAM_READ:
    case SNES::CGRAM_READ:
      if(!(bp[i].flags & FLAG_R))continue;
      if(bp[i].addr != addr)continue;
      if(message == SNES::MEM_READ && bp[i].source == DRAM);
      else if(message == SNES::SPCRAM_READ && bp[i].source == SPCRAM);
      else if(message == SNES::VRAM_READ && bp[i].source == VRAM);
      else if(message == SNES::OAM_READ && bp[i].source == OAM);
      else if(message == SNES::CGRAM_READ && bp[i].source == CGRAM);
      else continue;
      if(bp[i].flags & FLAG_V) {
        if(bp[i].value != value)continue;
      }
      dprintf("* Breakpoint %d hit (read)", i);
      bp[i].hit_count++;
      bp_hit = true;
      w_bp->refresh();
      break;
    case SNES::MEM_WRITE:
    case SNES::SPCRAM_WRITE:
    case SNES::VRAM_WRITE:
    case SNES::OAM_WRITE:
    case SNES::CGRAM_WRITE:
      if(!(bp[i].flags & FLAG_W))continue;
      if(bp[i].addr != addr)continue;
      if(message == SNES::MEM_WRITE && bp[i].source == DRAM);
      else if(message == SNES::SPCRAM_WRITE && bp[i].source == SPCRAM);
      else if(message == SNES::VRAM_WRITE && bp[i].source == VRAM);
      else if(message == SNES::OAM_WRITE && bp[i].source == OAM);
      else if(message == SNES::CGRAM_WRITE && bp[i].source == CGRAM);
      else continue;
      if(bp[i].flags & FLAG_V) {
        if(bp[i].value != value)continue;
      }
      dprintf("* Breakpoint %d hit (write)", i);
      bp[i].hit_count++;
      bp_hit = true;
      w_bp->refresh();
      break;
    }
    if(bp_hit == true) {
      bp_hit_num = i;
    }
  }
}

long __stdcall wndproc_bp(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
char t[256];
uint32 n, addr;
HDC hdc;
  switch(msg) {
  case WM_CTLCOLORSTATIC:
    if((HWND)lparam != GetDlgItem(hwnd, BREAKPOINT_LIST))break;
    hdc = (HDC)wparam;
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    return (long)hbr_backbrush;
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case BREAKPOINT_SET:
      n = SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_GETCURSEL, 0, 0) & 15;
      GetDlgItemText(hwnd, BREAKPOINT_OFFSET, t, 255);
      w_bp->bp[n].addr = strhex(t) & 0xffffff;
      GetDlgItemText(hwnd, BREAKPOINT_VALUE, t, 255);
      w_bp->bp[n].value = strhex(t) & 0xff;
      w_bp->bp[n].source = SendDlgItemMessage(hwnd, BREAKPOINT_SOURCE, CB_GETCURSEL, 0, 0);
      w_bp->bp[n].flags = BreakpointEditor::FLAG_NONE;
      if(SendDlgItemMessage(hwnd, BREAKPOINT_R, BM_GETCHECK, 0, 0)) {
        w_bp->bp[n].flags |= BreakpointEditor::FLAG_R;
      }
      if(SendDlgItemMessage(hwnd, BREAKPOINT_W, BM_GETCHECK, 0, 0)) {
        w_bp->bp[n].flags |= BreakpointEditor::FLAG_W;
      }
      if(SendDlgItemMessage(hwnd, BREAKPOINT_X, BM_GETCHECK, 0, 0)) {
        w_bp->bp[n].flags |= BreakpointEditor::FLAG_X;
      }
      if(SendDlgItemMessage(hwnd, BREAKPOINT_V, BM_GETCHECK, 0, 0)) {
        w_bp->bp[n].flags |= BreakpointEditor::FLAG_V;
      }
      w_bp->bp[n].hit_count = 0;
      w_bp->bp[n].set = (w_bp->bp[n].flags != BreakpointEditor::FLAG_NONE)?true:false;
      w_bp->refresh();
      break;
    case BREAKPOINT_CLEAR:
      n = SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_GETCURSEL, 0, 0) & 15;
      w_bp->bp[n].set = false;
      w_bp->refresh();
      break;
    case BREAKPOINT_ENABLE:
      if(w_bp->bp_enabled == false) {
        w_bp->bp_enabled = true;
        SendDlgItemMessage(hwnd, BREAKPOINT_ENABLE, BM_SETCHECK, (WPARAM)true, 0);
      } else {
        w_bp->bp_enabled = false;
        SendDlgItemMessage(hwnd, BREAKPOINT_ENABLE, BM_SETCHECK, (WPARAM)false, 0);
      }
      break;
    case BREAKPOINT_CLEARALL:
      for(n=0;n<16;n++) {
        w_bp->bp[n].set = false;
      }
      w_bp->refresh();
      break;
    }
    break;
/* Usually, bsnes will catch the close request here, and replace it
   with a hide command instead. */
  case WM_CLOSE:
    w_bp->hide();
    return 0;
/* Just in case something sends a DestroyWindow command to bsnes... */
  case WM_DESTROY:
    alert("Error: bsnes debugging breakpoint editor window has been destroyed!\n"
      "The emulator will still run, but it must be restarted before\n"
      "the debugger will function again.");
    bsnes->debugger_disable();
    return 0;
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void BreakpointEditor::clear() {
  SetDlgItemText(hwnd, BREAKPOINT_LIST, "");
}

void BreakpointEditor::refresh() {
char s[256 * 16], t[256], source[256], v[16];
  if(rom_image->loaded() == false)return;
  if(visible == false)return;

  strcpy(s, "");
  for(int i=0;i<16;i++) {
    if(bp[i].set == true) {
      switch(bp[i].source) {
      case DRAM:  strcpy(source, "DRAM  ");break;
      case SPCRAM:strcpy(source, "SPCRAM");break;
      case VRAM:  strcpy(source, "VRAM  ");break;
      case OAM:   strcpy(source, "OAM   ");break;
      case CGRAM: strcpy(source, "CGRAM ");break;
      default:    strcpy(source, "??????");break;
      }
      if(bp[i].flags & FLAG_V)sprintf(v, "%0.2x", bp[i].value);
      else strcpy(v, "--");
      sprintf(t, "%0.2d: %0.6x %c%c%c%c %s %s %10d",
        i, bp[i].addr,
        (bp[i].flags & FLAG_R)?'R':'r', (bp[i].flags & FLAG_W)?'W':'w',
        (bp[i].flags & FLAG_X)?'X':'x', (bp[i].flags & FLAG_V)?'V':'v',
        v, source, bp[i].hit_count);
    } else {
      sprintf(t, "%0.2d: ------ ---- -- ------ (disabled)", i);
    }
    strcat(s, t);
    if(i != 15)strcat(s, "\r\n");
  }
  SetDlgItemText(hwnd, BREAKPOINT_LIST, s);
}

void BreakpointEditor::create() {
WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = wndproc_bp;
  wc.lpszClassName = "bsnes_breakpoint";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  hwnd = CreateWindow("bsnes_breakpoint", "bsnes Breakpoint Editor",
    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    0, 0, 395, 245,
    0, 0, GetModuleHandle(0), 0);

  resize(395, 245);
  to_left();
  to_bottom(w_console->hwnd);

  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY,
    5, 5, 260, 235, hwnd, (HMENU)BREAKPOINT_LIST, GetModuleHandle(0), 0);

  CreateWindow("STATIC", "Breakpoint:  Offset:", WS_CHILD | WS_VISIBLE,
    270, 5, 120, 15, hwnd, (HMENU)BREAKPOINT_STATIC1, GetModuleHandle(0), 0);

  CreateWindow("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    270, 20, 60, 300, hwnd, (HMENU)BREAKPOINT_NUM, GetModuleHandle(0), 0);
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"00");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"01");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"02");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"03");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"04");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"05");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"06");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"07");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"08");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"09");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"10");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"11");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"12");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"13");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"14");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_ADDSTRING, 0, (LPARAM)"15");
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM, CB_SETCURSEL, 0, 0);

  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "000000", WS_CHILD | WS_VISIBLE,
    330, 20, 60, 22, hwnd, (HMENU)BREAKPOINT_OFFSET, GetModuleHandle(0), 0);

  CreateWindow("BUTTON", "R", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    270, 45, 30, 15, hwnd, (HMENU)BREAKPOINT_R, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "W", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    300, 45, 30, 15, hwnd, (HMENU)BREAKPOINT_W, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "X", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    330, 45, 30, 15, hwnd, (HMENU)BREAKPOINT_X, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "V", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    360, 45, 30, 15, hwnd, (HMENU)BREAKPOINT_V, GetModuleHandle(0), 0);

  CreateWindow("STATIC", "Value:  Source:", WS_CHILD | WS_VISIBLE,
    270, 60, 120, 15, hwnd, (HMENU)BREAKPOINT_STATIC2, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "00", WS_CHILD | WS_VISIBLE,
    270, 75, 35, 21, hwnd, (HMENU)BREAKPOINT_VALUE, GetModuleHandle(0), 0);
  CreateWindow("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    305, 75, 85, 200, hwnd, (HMENU)BREAKPOINT_SOURCE, GetModuleHandle(0), 0);
  SendDlgItemMessage(hwnd, BREAKPOINT_SOURCE, CB_ADDSTRING, 0, (LPARAM)"DRAM");
  SendDlgItemMessage(hwnd, BREAKPOINT_SOURCE, CB_ADDSTRING, 0, (LPARAM)"SPCRAM");
  SendDlgItemMessage(hwnd, BREAKPOINT_SOURCE, CB_ADDSTRING, 0, (LPARAM)"VRAM");
  SendDlgItemMessage(hwnd, BREAKPOINT_SOURCE, CB_ADDSTRING, 0, (LPARAM)"OAM");
  SendDlgItemMessage(hwnd, BREAKPOINT_SOURCE, CB_ADDSTRING, 0, (LPARAM)"CGRAM");
  SendDlgItemMessage(hwnd, BREAKPOINT_SOURCE, CB_SETCURSEL, 0, 0);

  CreateWindow("BUTTON", "Set BP", WS_CHILD | WS_VISIBLE,
    270, 100, 60, 20, hwnd, (HMENU)BREAKPOINT_SET, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Clear BP", WS_CHILD | WS_VISIBLE,
    330, 100, 60, 20, hwnd, (HMENU)BREAKPOINT_CLEAR, GetModuleHandle(0), 0);

  CreateWindow("BUTTON", "Enable Breakpoints", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    270, 163, 120, 15, hwnd, (HMENU)BREAKPOINT_ENABLE, GetModuleHandle(0), 0);
  SendDlgItemMessage(hwnd, BREAKPOINT_ENABLE, BM_SETCHECK, (WPARAM)true, 0);
  CreateWindow("BUTTON", "Clear All Breakpoints", WS_CHILD | WS_VISIBLE,
    270, 180, 120, 20, hwnd, (HMENU)BREAKPOINT_CLEARALL, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Import...", WS_CHILD | WS_VISIBLE | WS_DISABLED,
    270, 200, 120, 20, hwnd, (HMENU)BREAKPOINT_IMPORT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Export...", WS_CHILD | WS_VISIBLE | WS_DISABLED,
    270, 220, 120, 20, hwnd, (HMENU)BREAKPOINT_EXPORT, GetModuleHandle(0), 0);

  SendDlgItemMessage(hwnd, BREAKPOINT_LIST,     WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_STATIC1,  WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_NUM,      WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_OFFSET,   WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_R,        WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_W,        WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_X,        WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_V,        WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_STATIC2,  WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_VALUE,    WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_SOURCE,   WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_SET,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_CLEAR,    WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_ENABLE,   WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_CLEARALL, WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_EXPORT,   WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, BREAKPOINT_IMPORT,   WM_SETFONT, (WPARAM)hFont,     TRUE);

HWND h;
uint32 style;
  for(int i=0;i<100;i++) {
    ctl_disabled[i] = false;
    h = GetDlgItem(hwnd, i + 100);
    if(!h)continue;
    style = GetWindowLong(h, GWL_STYLE);
    if(style & WS_DISABLED)ctl_disabled[i] = true;
  }
}

BreakpointEditor::BreakpointEditor() {
int i;
  for(i=0;i<16;i++) {
    bp[i].addr      = 0x000000;
    bp[i].value     = 0x00;
    bp[i].source    = DRAM;
    bp[i].flags     = FLAG_NONE;
    bp[i].hit_count = 0;
    bp[i].set       = false;
  }
  bp_enabled = true;
  bp_hit = false;
}
