long __stdcall wndproc_memoryeditor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
char t[256];
uint32 addr, value, pos;
  switch(msg) {
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case MEMORYEDITOR_MODE:
      if(HIWORD(wparam) == CBN_SELCHANGE) {
        pos = SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE, CB_GETCURSEL, 0, 0);
        if(pos == 0) {
          w_memory->edit_mode = MemoryEditor::MODE_DRAM;
          w_memory->edit_addr = 0x7e0000;
          w_memory->edit_mask = 0xffffff;
        } else if(pos == 1) {
          w_memory->edit_mode = MemoryEditor::MODE_DRAM;
          w_memory->edit_addr = 0x008000;
          w_memory->edit_mask = 0xffffff;
        } else if(pos == 2) {
          w_memory->edit_mode = MemoryEditor::MODE_DRAM;
          w_memory->edit_addr = 0x700000;
          w_memory->edit_mask = 0xffffff;
        } else if(pos == 3) {
          w_memory->edit_mode = MemoryEditor::MODE_VRAM;
          w_memory->edit_addr = 0x0000;
          w_memory->edit_mask = 0xffff;
        } else if(pos == 4) {
          w_memory->edit_mode = MemoryEditor::MODE_OAM;
          w_memory->edit_addr = 0x0000;
          w_memory->edit_mask = 0x03ff;
        } else if(pos == 5) {
          w_memory->edit_mode = MemoryEditor::MODE_CGRAM;
          w_memory->edit_addr = 0x0000;
          w_memory->edit_mask = 0x01ff;
        }
        w_memory->refresh();
      }
      break;
    case MEMORYEDITOR_GOTO:
      GetDlgItemText(hwnd, MEMORYEDITOR_GOTOADDR, t, 255);
      addr = strhex(t) & w_memory->edit_mask;
      w_memory->edit_addr = addr;
      w_memory->refresh();
      break;
    case MEMORYEDITOR_EDIT:
      GetDlgItemText(hwnd, MEMORYEDITOR_OFFSET, t, 255);
      addr = strhex(t) & w_memory->edit_mask;
      GetDlgItemText(hwnd, MEMORYEDITOR_VALUE, t, 255);
      value = strhex(t) & 0xff;
      switch(w_memory->edit_mode) {
      case MemoryEditor::MODE_DRAM: bsnes->write(bSNES::DRAM,  addr, value);break;
      case MemoryEditor::MODE_VRAM: bsnes->write(bSNES::VRAM,  addr, value);break;
      case MemoryEditor::MODE_OAM:  bsnes->write(bSNES::OAM,   addr, value);break;
      case MemoryEditor::MODE_CGRAM:bsnes->write(bSNES::CGRAM, addr, value);break;
      }
      break;
    case MEMORYEDITOR_FEXPORT:
      pos = SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE, CB_GETCURSEL, 0, 0);
      w_memory->export(pos);
      break;
    case MEMORYEDITOR_UP40:
      w_memory->edit_addr -= 0x40;
      w_memory->edit_addr &= w_memory->edit_mask;
      w_memory->refresh();
      break;
    case MEMORYEDITOR_DOWN40:
      w_memory->edit_addr += 0x40;
      w_memory->edit_addr &= w_memory->edit_mask;
      w_memory->refresh();
      break;
    case MEMORYEDITOR_UP400:
      w_memory->edit_addr -= 0x400;
      w_memory->edit_addr &= w_memory->edit_mask;
      w_memory->refresh();
      break;
    case MEMORYEDITOR_DOWN400:
      w_memory->edit_addr += 0x400;
      w_memory->edit_addr &= w_memory->edit_mask;
      w_memory->refresh();
      break;
    case MEMORYEDITOR_UP4000:
      w_memory->edit_addr -= 0x4000;
      w_memory->edit_addr &= w_memory->edit_mask;
      w_memory->refresh();
      break;
    case MEMORYEDITOR_DOWN4000:
      w_memory->edit_addr += 0x4000;
      w_memory->edit_addr &= w_memory->edit_mask;
      w_memory->refresh();
      break;
    case MEMORYEDITOR_UP40000:
      w_memory->edit_addr -= 0x40000;
      w_memory->edit_addr &= w_memory->edit_mask;
      w_memory->refresh();
      break;
    case MEMORYEDITOR_DOWN40000:
      w_memory->edit_addr += 0x40000;
      w_memory->edit_addr &= w_memory->edit_mask;
      w_memory->refresh();
      break;
    }
    break;
/* Usually, bsnes will catch the close request here, and replace it
   with a hide command instead. */
  case WM_CLOSE:
    w_memory->hide();
    return 0;
/* Just in case something sends a DestroyWindow command to bsnes... */
  case WM_DESTROY:
    alert("Error: bsnes debugging memory editor window has been destroyed!\n"
      "The emulator will still run, but it must be restarted before\n"
      "the debugger will function again.");
    bsnes->debugger_disable();
    return 0;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void CreateWindowMemory() {
WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = wndproc_memoryeditor;
  wc.lpszClassName = "bsnes_memoryeditor";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  w_memory->hwnd = CreateWindow("bsnes_memoryeditor", "bsnes memory editor",
    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    0, 0, 500, 245,
    0, 0, GetModuleHandle(0), 0);

  w_memory->resize(500, 245);
  w_memory->to_left(w_bp->hwnd);
  w_memory->to_bottom(w_console->hwnd);

  w_memory->create_controls();
}

void MemoryEditor::export(uint32 type) {
FILE *fp;
int i, x;
  if(type == 0) { //DRAM
    fp = fopen("dram.bin", "wb");
    for(i=0x7e0000;i<=0x7fffff;i++) {
      x = bsnes->read(bSNES::DRAM, i);
      fputc(x, fp);
    }
    fclose(fp);
  } else if(type == 1) { //SRAM
  /* should actually try and get the correct SRAM size here... */
    fp = fopen("sram.bin", "wb");
    for(i=0x700000;i<=0x720000;i++) {
      x = bsnes->read(bSNES::DRAM, i);
      fputc(x, fp);
    }
    fclose(fp);
  } else if(type == 2) { //VRAM
    fp = fopen("vram.bin", "wb");
    for(i=0x0000;i<=0xffff;i++) {
      x = bsnes->read(bSNES::VRAM, i);
      fputc(x, fp);
    }
    fclose(fp);
  } else if(type == 3) { //OAM
    fp = fopen("oam.bin", "wb");
    for(i=0x0000;i<=0x021f;i++) {
      x = bsnes->read(bSNES::OAM, i);
      fputc(x, fp);
    }
    fclose(fp);
  } else if(type == 4) { //CGRAM
    fp = fopen("cgram.bin", "wb");
    for(i=0x0000;i<=0x01ff;i++) {
      x = bsnes->read(bSNES::CGRAM, i);
      fputc(x, fp);
    }
    fclose(fp);
  } else if(type == 5) { //All
    export(0);
    export(1);
    export(2);
    export(3);
    export(4);
  }
}

uint8 MemoryEditor::read_byte(uint32 addr) {
  switch(edit_mode) {
  case MODE_DRAM: return bsnes->read(bSNES::DRAM,  addr);
  case MODE_VRAM: return bsnes->read(bSNES::VRAM,  addr);
  case MODE_OAM:  return bsnes->read(bSNES::OAM,   addr);
  case MODE_CGRAM:return bsnes->read(bSNES::CGRAM, addr);
  }
  return 0x00;
}

void MemoryEditor::clear() {
  SetDlgItemText(hwnd, MEMORYEDITOR_VIEW, "");
}

void MemoryEditor::refresh(uint32 type, uint32 addr) {
char s[4096], t[256];
int x, y;
  if(rom_image->loaded() == false)return;
  if(visible == false)return;

  if(type == SNES::MEM_WRITE) {
    if(edit_mode != MODE_DRAM)return;
    if(addr < edit_addr || addr >= edit_addr + 256)return;
  } else if(type == SNES::VRAM_WRITE) {
    if(edit_mode != MODE_VRAM)return;
    if(addr < edit_addr || addr >= edit_addr + 256)return;
  } else if(type == SNES::OAM_WRITE) {
    if(edit_mode != MODE_OAM)return;
    if(addr < edit_addr || addr >= edit_addr + 256)return;
  } else if(type == SNES::CGRAM_WRITE) {
    if(edit_mode != MODE_CGRAM)return;
    if(addr < edit_addr || addr >= edit_addr + 256)return;
  }

  strcpy(s, "");
  for(y=0;y<16;y++) {
    sprintf(t, "%0.6x: ", (edit_addr + y * 16) & edit_mask);
    strcat(s, t);
    for(x=0;x<16;x++) {
      sprintf(t, "%0.2x", read_byte(edit_addr + y * 16 + x));
      strcat(s, t);
      if(x != 15)strcat(s, " ");
    }
    if(y != 15)strcat(s, "\r\n");
  }
  SetDlgItemText(hwnd, MEMORYEDITOR_VIEW, s);
}

void MemoryEditor::create_controls() {
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY,
      5,   5, 395, 235, hwnd, (HMENU)MEMORYEDITOR_VIEW, GetModuleHandle(0), 0);

  CreateWindow("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    405,   5,  90, 200, hwnd, (HMENU)MEMORYEDITOR_MODE, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "000000", WS_CHILD | WS_VISIBLE,
    405,  26,  55,  20, hwnd, (HMENU)MEMORYEDITOR_GOTOADDR, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Go", WS_CHILD | WS_VISIBLE,
    460,  26,  35,  20, hwnd, (HMENU)MEMORYEDITOR_GOTO, GetModuleHandle(0), 0);

  CreateWindow("STATIC", "Offset:       Value:", WS_CHILD | WS_VISIBLE,
    405,  50,  90,  15, hwnd, (HMENU)MEMORYEDITOR_STATIC1, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "000000", WS_CHILD | WS_VISIBLE,
    405,  65,  55,  20, hwnd, (HMENU)MEMORYEDITOR_OFFSET, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "00", WS_CHILD | WS_VISIBLE,
    460,  65,  35,  20, hwnd, (HMENU)MEMORYEDITOR_VALUE, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Edit Memory", WS_CHILD | WS_VISIBLE,
    405,  85,  90,  20, hwnd, (HMENU)MEMORYEDITOR_EDIT, GetModuleHandle(0), 0);

  CreateWindow("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    405, 110,  90, 200, hwnd, (HMENU)MEMORYEDITOR_FSOURCE, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Export...", WS_CHILD | WS_VISIBLE,
    405, 131,  90,  20, hwnd, (HMENU)MEMORYEDITOR_FEXPORT, GetModuleHandle(0), 0);

  CreateWindow("BUTTON", "-40", WS_CHILD | WS_VISIBLE,
    405, 160,  45,  20, hwnd, (HMENU)MEMORYEDITOR_UP40, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "+40", WS_CHILD | WS_VISIBLE,
    450, 160,  45,  20, hwnd, (HMENU)MEMORYEDITOR_DOWN40, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "-400", WS_CHILD | WS_VISIBLE,
    405, 180,  45,  20, hwnd, (HMENU)MEMORYEDITOR_UP400, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "+400", WS_CHILD | WS_VISIBLE,
    450, 180,  45,  20, hwnd, (HMENU)MEMORYEDITOR_DOWN400, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "-4000", WS_CHILD | WS_VISIBLE,
    405, 200,  45,  20, hwnd, (HMENU)MEMORYEDITOR_UP4000, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "+4000", WS_CHILD | WS_VISIBLE,
    450, 200,  45,  20, hwnd, (HMENU)MEMORYEDITOR_DOWN4000, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "-40000", WS_CHILD | WS_VISIBLE,
    405, 220,  45,  20, hwnd, (HMENU)MEMORYEDITOR_UP40000, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "+40000", WS_CHILD | WS_VISIBLE,
    450, 220,  45,  20, hwnd, (HMENU)MEMORYEDITOR_DOWN40000, GetModuleHandle(0), 0);

  SendDlgItemMessage(hwnd, MEMORYEDITOR_VIEW,      WM_SETFONT, (WPARAM)hMonofont, 0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE,      WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_GOTOADDR,  WM_SETFONT, (WPARAM)hMonofont, 0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_GOTO,      WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_STATIC1,   WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_OFFSET,    WM_SETFONT, (WPARAM)hMonofont, 0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_VALUE,     WM_SETFONT, (WPARAM)hMonofont, 0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_EDIT,      WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE,   WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_FEXPORT,   WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_UP40,      WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_DOWN40,    WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_UP400,     WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_DOWN400,   WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_UP4000,    WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_DOWN4000,  WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_UP40000,   WM_SETFONT, (WPARAM)hFont,     0);
  SendDlgItemMessage(hwnd, MEMORYEDITOR_DOWN40000, WM_SETFONT, (WPARAM)hFont,     0);

  SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE, CB_ADDSTRING, 0, (LPARAM)"DRAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE, CB_ADDSTRING, 0, (LPARAM)"ROM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE, CB_ADDSTRING, 0, (LPARAM)"SRAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE, CB_ADDSTRING, 0, (LPARAM)"VRAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE, CB_ADDSTRING, 0, (LPARAM)"OAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE, CB_ADDSTRING, 0, (LPARAM)"CGRAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_MODE, CB_SETCURSEL, 0, 0);

  SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE, CB_ADDSTRING, 0, (LPARAM)"DRAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE, CB_ADDSTRING, 0, (LPARAM)"SRAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE, CB_ADDSTRING, 0, (LPARAM)"VRAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE, CB_ADDSTRING, 0, (LPARAM)"OAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE, CB_ADDSTRING, 0, (LPARAM)"CGRAM");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE, CB_ADDSTRING, 0, (LPARAM)"All");
  SendDlgItemMessage(hwnd, MEMORYEDITOR_FSOURCE, CB_SETCURSEL, 0, 0);

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
