#define __console_joypad_lock(n, l) \
  if(w_console->joypad_lock.##n## == true) { \
    w_console->joypad_lock.##n## = false; \
    SendDlgItemMessage(hwnd, CONSOLE_CFGLOCK##l##, BM_SETCHECK, (WPARAM)false, 0); \
  } else { \
    w_console->joypad_lock.##n## = true; \
    SendDlgItemMessage(hwnd, CONSOLE_CFGLOCK##l##, BM_SETCHECK, (WPARAM)true, 0); \
  }

long Console::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
char t[256];
int i, pos;
uint32 addr, len, value;
HDC hdc;
  switch(msg) {
  case WM_CTLCOLORSTATIC:
    if((HWND)lparam != GetDlgItem(hwnd, CONSOLE_STATUS))break;
    hdc = (HDC)wparam;
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    return (long)hbr_backbrush;
  case WM_COMMAND:
    switch(LOWORD(wparam)) {
    case CONSOLE_CPUSTEP:
      if(bsnes->get_status() == bSNES::STOP) {
        bsnes->set_status(bSNES::RUNTOCPUSTEP);
      }
      break;
    case CONSOLE_CPUPROCEED:
      if(bsnes->get_status() == bSNES::STOP) {
        bsnes->set_status(bSNES::RUNTOCPUPROCEED);
      }
      break;
    case CONSOLE_CPUSKIP:
      if(bsnes->get_status() == bSNES::STOP) {
        cpu->regs.pc.w += cpu->opcode_length();
        bsnes->disassemble_cpu_op();
      }
      break;
    case CONSOLE_CPUTRACE:
      if(bsnes->get_status() == bSNES::STOP) {
        bsnes->status.cpu_trace_pos = 0;
        GetDlgItemText(hwnd, CONSOLE_CPUTRACENUM, t, 255);
        if(!strcmp(t, "") || strdec(t) == 0)break;
        bsnes->status.cpu_trace_stop = strdec(t) & 0xffffff;
        bsnes->set_status(bSNES::RUNTOCPUTRACE);
      }
      break;
    case CONSOLE_CPUDISABLE:
      if(bsnes->get_status() == bSNES::STOP) {
        addr = cpu->regs.pc.d;
        len = cpu->opcode_length();
        for(i=0;i<len;i++) {
          bsnes->write(bSNES::DRAM, (addr & 0xff0000) | ((addr + i) & 0xffff), 0xea);
        }
//      cpu->regs.pc.w += len;
        bsnes->disassemble_cpu_op();
      }
      break;
    case CONSOLE_APUSTEP:
      if(bsnes->get_status() == bSNES::STOP) {
        bsnes->set_status(bSNES::RUNTOAPUSTEP);
      }
      break;
    case CONSOLE_SYSRUN:
      if(bsnes->get_status() == bSNES::STOP) {
        is_running(true);
        bsnes->set_status(bSNES::RUNTOSIGNAL);
      } else {
        is_running(false);
        bsnes->set_status(bSNES::STOP);
        bsnes->disassemble_apu_op();
        bsnes->disassemble_cpu_op();
      }
      break;
    case CONSOLE_SYSRUNTOFRAME:
      if(bsnes->get_status() == bSNES::STOP) {
        bsnes->set_status(bSNES::RUNTOFRAME);
      }
      break;
    case CONSOLE_CFGOUTPUTCPU:
      outputcpu ^= 1;
      SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTCPU, BM_SETCHECK, (WPARAM)outputcpu, 0);
      break;
    case CONSOLE_CFGOUTPUTAPU:
      outputapu ^= 1;
      SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTAPU, BM_SETCHECK, (WPARAM)outputapu, 0);
      break;
    case CONSOLE_CFGOUTPUTDBG:
      outputdbg ^= 1;
      SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTDBG, BM_SETCHECK, (WPARAM)outputdbg, 0);
      break;
    case CONSOLE_CFGTRACE:
      if(tracing_enabled == false) {
        write("* Tracing enabled");
        tracing_enabled = true;
        log_fp = fopen("trace.log", "wb");
        bsnes->disassemble_apu_op();
        bsnes->disassemble_cpu_op();
        SendDlgItemMessage(hwnd, CONSOLE_CFGTRACE, BM_SETCHECK, (WPARAM)true, 0);
      } else {
        tracing_enabled = false;
        fclose(log_fp);
        SendDlgItemMessage(hwnd, CONSOLE_CFGTRACE, BM_SETCHECK, (WPARAM)false, 0);
        write("* Tracing disabled");
      }
      break;
    case CONSOLE_CFGREGSET:
      GetDlgItemText(hwnd, CONSOLE_CFGREGVAL, t, 255);
      value = strhex(t);
      pos = SendDlgItemMessage(hwnd, CONSOLE_CFGREGTYPE, CB_GETCURSEL, 0, 0);
      if(pos == 0) { //Set CPU register
        pos = SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_GETCURSEL, 0, 0);
        switch(pos) {
        case 0:cpu->regs.a.w  = value;break;
        case 1:cpu->regs.x.w  = value;break;
        case 2:cpu->regs.y.w  = value;break;
        case 3:cpu->regs.s.w  = value;break;
        case 4:cpu->regs.d.w  = value;break;
        case 5:cpu->regs.db   = value;break;
        case 6:cpu->regs.p    = value;break;
        case 7:cpu->regs.e    = value;break;
        case 8:cpu->regs.pc.d = value;break;
        }
      //these bits can never be clear in emulation mode
        if(cpu->regs.e)cpu->regs.p |= 0x30;
        bsnes->disassemble_cpu_op();
      } else { //Set APU register
      }
      break;
    case CONSOLE_CFGLOCKUP:    __console_joypad_lock(up,     UP);    break;
    case CONSOLE_CFGLOCKDOWN:  __console_joypad_lock(down,   DOWN);  break;
    case CONSOLE_CFGLOCKLEFT:  __console_joypad_lock(left,   LEFT);  break;
    case CONSOLE_CFGLOCKRIGHT: __console_joypad_lock(right,  RIGHT); break;
    case CONSOLE_CFGLOCKA:     __console_joypad_lock(a,      A);     break;
    case CONSOLE_CFGLOCKB:     __console_joypad_lock(b,      B);     break;
    case CONSOLE_CFGLOCKX:     __console_joypad_lock(x,      X);     break;
    case CONSOLE_CFGLOCKY:     __console_joypad_lock(y,      Y);     break;
    case CONSOLE_CFGLOCKL:     __console_joypad_lock(l,      L);     break;
    case CONSOLE_CFGLOCKR:     __console_joypad_lock(r,      R);     break;
    case CONSOLE_CFGLOCKSELECT:__console_joypad_lock(select, SELECT);break;
    case CONSOLE_CFGLOCKSTART: __console_joypad_lock(start,  START); break;
    }
    break;
  case WM_CLOSE:
    bsnes->debugger_disable();
    return 0;
  case WM_DESTROY:
    alert("Error: bsnes debugging console window has been destroyed!\n"
      "The emulator will still run, but it must be restarted before\n"
      "the debugger will function again.");
    bsnes->debugger_disable();
    return 0;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

long __stdcall wndproc_console(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return w_console->wndproc(hwnd, msg, wparam, lparam);
}

void Console::clear() {
static uint8 linecol[4] = { 0, 0, 0 };
  for(int i=0;i<CONSOLE_LINES;i++) {
    memset(output_line[i], 0x20, 80);
    output_line[i][80] = 0;
  }
  memset(output_linecol, 0, CONSOLE_LINES);
  editex_settext(GetDlgItem(hwnd, CONSOLE_OUTPUT), "", output_linecol, EDITEX_ALIGNBOTTOM);
  editex_settext(GetDlgItem(hwnd, CONSOLE_STATUS), "", linecol, EDITEX_ALIGNTOP);
}

bool Console::can_write(uint32 message_type) {
  if(message_type == Console::CPU_MESSAGE   && outputcpu == false)return false;
  if(message_type == Console::APU_MESSAGE   && outputapu == false)return false;
  if(message_type == Console::DEBUG_MESSAGE && outputdbg == false)return false;

  if(message_type == Console::DEBUG_MESSAGE)return true;

  switch(bsnes->get_status()) {
  case bSNES::RUNTOCPUPROCEED:
  case bSNES::RUNTOCPUTRACE:
  case bSNES::RUNTOSIGNAL:
  case bSNES::RUNTOFRAME:
    return false;
  }
  return true;
}

void Console::write(char *s, uint32 message_type) {
char t[128];
bool blank_line;
  if(tracing_enabled == true) {
    if     (message_type == Console::CPU_MESSAGE   && outputcpu == false);
    else if(message_type == Console::APU_MESSAGE   && outputapu == false);
    else if(message_type == Console::DEBUG_MESSAGE && outputdbg == false);
    else fprintf(log_fp, "%s\r\n", s);
  }

  if(!can_write(message_type)) {
    return;
  }

  blank_line = false;
int sl = strlen(s);
  if(sl >= 80) {
    memcpy(t, s, 80);
  } else {
    blank_line = (sl == 0);
    strcpy(t, s);
    memset(t + sl, 0x20, 80 - sl);
  }
  t[80] = 0;
//only allow ascii characters. other characters will force the
//font to change to one that supports non-ascii characters,
//which will break the line highlighting and alignment of text
  for(int i=0;i<80;i++) {
    if(t[i] & 0x80)t[i] = '?';
  }

  for(i=0;i<CONSOLE_LINES - 1;i++) {
    output_linecol[i] = output_linecol[i + 1];
    strcpy(output_line[i], output_line[i + 1]);
  }
  if(blank_line == true) {
    output_linecol[i] = 0;
  } else {
    switch(message_type) {
    case Console::CPU_MESSAGE:  output_linecol[i] = 1;break;
    case Console::APU_MESSAGE:  output_linecol[i] = 2;break;
    case Console::DEBUG_MESSAGE:output_linecol[i] = 3;break;
    }
  }
  strcpy(output_line[i], t);

  strcpy(output_data, "");
  for(i=0;i<CONSOLE_LINES;i++) {
    strcat(output_data, output_line[i]);
    if(i != CONSOLE_LINES - 1)strcat(output_data, "\n");
  }

  editex_settext(GetDlgItem(hwnd, CONSOLE_OUTPUT), output_data, output_linecol, EDITEX_ALIGNBOTTOM);
}

void Console::is_running(bool n) {
  if(n == true) {
    if(_is_running == false) {
      _is_running = true;
      SetDlgItemText(hwnd, CONSOLE_SYSRUN, "Stop");
    }
  } else { //n == false
    if(_is_running == true) {
      _is_running = false;
      SetDlgItemText(hwnd, CONSOLE_SYSRUN, "Run");
    }
  }
}

void Console::update_status() {
char s[4096], t[512];
static uint8 linecol[4] = { 1, 2, 3 };
  if(rom_image->loaded() == false)return;

  strcpy(s, "");
  sprintf(t, "V:%3d H:%3d HC:%4d I:%d IF:%d O:%d",
    cpu->vcounter(), cpu->hcounter(), cpu->hcycles(),
    cpu->interlace(), cpu->interlace_field(), cpu->overscan());
  strcat(s, t);
  if(cfg.apu.enabled) {
    sprintf(t, " -- CPU[$%0.2x,$%0.2x,$%0.2x,$%0.2x]<>APU[$%0.2x,$%0.2x,$%0.2x,$%0.2x]",
      cpu->port_read(0), cpu->port_read(1), cpu->port_read(2), cpu->port_read(3),
      apu->port_read(0), apu->port_read(1), apu->port_read(2), apu->port_read(3)
    );
    strcat(s, t);
  }
  SetDlgItemText(hwnd, CONSOLE_STATUS, s);
}

void Console::set_reg_list_type(uint8 list_type) {
  switch(list_type) {
  case REGTYPE_CPU:
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"A");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"X");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"Y");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"S");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"D");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"DB");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"P");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"E");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_ADDSTRING, 0, (LPARAM)"PC");
    SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM, CB_SETCURSEL, 0, 0);
  }
}

void Console::create() {
WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = wndproc_console;
  wc.lpszClassName = "bsnes_console";
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  RegisterClass(&wc);

  hwnd = CreateWindow("bsnes_console", "bsnes Debug Console",
    WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
    0, 0, 847, 354,
    0, 0, GetModuleHandle(0), 0);

  resize(847, 354);
  to_left();
  to_bottom();

int x, y;
  CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
    5, 5, 582, 285,
    hwnd, (HMENU)CONSOLE_OUTPUT, GetModuleHandle(0), 0);
  editex_setbackcolor(GetDlgItem(hwnd, CONSOLE_OUTPUT), RGB(32, 32, 32));
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY,
    5, 290, 582, 59,
    hwnd, (HMENU)CONSOLE_STATUS, GetModuleHandle(0), 0);

  x = 592;
  y =   5;
  CreateWindow("BUTTON", "cpu (wdc 65c816)", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
    x, y, 250, 60, hwnd, (HMENU)CONSOLE_CPUGROUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Step",    WS_CHILD | WS_VISIBLE,               x +   5, y + 15, 80, 20, hwnd, (HMENU)CONSOLE_CPUSTEP,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Proceed", WS_CHILD | WS_VISIBLE,               x +  85, y + 15, 80, 20, hwnd, (HMENU)CONSOLE_CPUPROCEED, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Skip",    WS_CHILD | WS_VISIBLE,               x + 165, y + 15, 80, 20, hwnd, (HMENU)CONSOLE_CPUSKIP,    GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE,
    x + 5, y + 35, 80, 20, hwnd, (HMENU)CONSOLE_CPUTRACENUM, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Trace",   WS_CHILD | WS_VISIBLE, x +  85, y + 35, 80, 20, hwnd, (HMENU)CONSOLE_CPUTRACE,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Disable", WS_CHILD | WS_VISIBLE, x + 165, y + 35, 80, 20, hwnd, (HMENU)CONSOLE_CPUDISABLE, GetModuleHandle(0), 0);

  y += 66;
  CreateWindow("BUTTON", "apu (sony spc700)", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
    x, y, 250, 60, hwnd, (HMENU)CONSOLE_APUGROUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Step",    WS_CHILD | WS_VISIBLE, x + 5, y + 15, 80, 20, hwnd, (HMENU)CONSOLE_APUSTEP,    GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Proceed", WS_CHILD | WS_VISIBLE | WS_DISABLED, x +  85, y + 15, 80, 20, hwnd, (HMENU)CONSOLE_APUPROCEED, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Skip",    WS_CHILD | WS_VISIBLE | WS_DISABLED, x + 165, y + 15, 80, 20, hwnd, (HMENU)CONSOLE_APUSKIP,    GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE | WS_DISABLED,
    x + 5, y + 35, 80, 20, hwnd, (HMENU)CONSOLE_APUTRACENUM, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Trace",   WS_CHILD | WS_VISIBLE | WS_DISABLED, x +  85, y + 35, 80, 20, hwnd, (HMENU)CONSOLE_APUTRACE,   GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Disable", WS_CHILD | WS_VISIBLE | WS_DISABLED, x + 165, y + 35, 80, 20, hwnd, (HMENU)CONSOLE_APUDISABLE, GetModuleHandle(0), 0);

  y += 66;
  CreateWindow("BUTTON", "System",    WS_CHILD | WS_VISIBLE | BS_GROUPBOX, x,       y,      250, 40, hwnd, (HMENU)CONSOLE_SYSGROUP,      GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Run",       WS_CHILD | WS_VISIBLE,               x +   5, y + 15,  80, 20, hwnd, (HMENU)CONSOLE_SYSRUN,        GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "To Frame",  WS_CHILD | WS_VISIBLE,               x +  85, y + 15,  80, 20, hwnd, (HMENU)CONSOLE_SYSRUNTOFRAME, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "To NMI",    WS_CHILD | WS_VISIBLE | WS_DISABLED, x + 165, y + 15,  80, 20, hwnd, (HMENU)CONSOLE_SYSRUNTONMI,   GetModuleHandle(0), 0);

  y += 46;
  CreateWindow("BUTTON", "Settings", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, x, y, 250, 166, hwnd, (HMENU)CONSOLE_CFGGROUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Output CPU instructions to console", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +   5, y + 15, 240,  15, hwnd, (HMENU)CONSOLE_CFGOUTPUTCPU, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Output APU instructions to console", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +   5, y + 30, 240,  15, hwnd, (HMENU)CONSOLE_CFGOUTPUTAPU, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Output debug messages to console", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +   5, y + 45, 240,  15, hwnd, (HMENU)CONSOLE_CFGOUTPUTDBG, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Trace console output to file", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +   5, y + 60, 240,  15, hwnd, (HMENU)CONSOLE_CFGTRACE, GetModuleHandle(0), 0);

  y += 77;
  CreateWindow("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    x +   5, y,  60, 200, hwnd, (HMENU)CONSOLE_CFGREGTYPE, GetModuleHandle(0), 0);
  CreateWindow("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    x +  65, y,  60, 200, hwnd, (HMENU)CONSOLE_CFGREGNUM, GetModuleHandle(0), 0);
  CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE,
    x + 125, y,  60,  21, hwnd, (HMENU)CONSOLE_CFGREGVAL, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Set Reg", WS_CHILD | WS_VISIBLE,
    x + 185, y,  60,  21, hwnd, (HMENU)CONSOLE_CFGREGSET, GetModuleHandle(0), 0);

  y += 24;
  CreateWindow("STATIC", "Lock Joypad Button(s) Down:", WS_CHILD | WS_VISIBLE, x + 5, y, 240, 15, hwnd, (HMENU)CONSOLE_CFGLOCK, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Up", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +   5, y + 15,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKUP, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Down", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +  65, y + 15,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKDOWN, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Left", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x + 125, y + 15,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKLEFT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Right", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x + 185, y + 15,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKRIGHT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "A", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +   5, y + 30,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKA, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "B", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +  65, y + 30,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKB, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "X", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x + 125, y + 30,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKX, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Y", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x + 185, y + 30,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKY, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "L", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +   5, y + 45,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKL, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "R", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x +  65, y + 45,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKR, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Select", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x + 125, y + 45,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKSELECT, GetModuleHandle(0), 0);
  CreateWindow("BUTTON", "Start", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x + 185, y + 45,  60,  15, hwnd, (HMENU)CONSOLE_CFGLOCKSTART, GetModuleHandle(0), 0);

  SendDlgItemMessage(hwnd, CONSOLE_OUTPUT,        WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_STATUS,        WM_SETFONT, (WPARAM)hMonofont, TRUE);

  SendDlgItemMessage(hwnd, CONSOLE_CPUGROUP,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CPUSTEP,       WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CPUPROCEED,    WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CPUSKIP,       WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CPUTRACENUM,   WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CPUTRACE,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CPUDISABLE,    WM_SETFONT, (WPARAM)hFont,     TRUE);

  SendDlgItemMessage(hwnd, CONSOLE_APUGROUP,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_APUSTEP,       WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_APUPROCEED,    WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_APUSKIP,       WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_APUTRACENUM,   WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_APUTRACE,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_APUDISABLE,    WM_SETFONT, (WPARAM)hFont,     TRUE);

  SendDlgItemMessage(hwnd, CONSOLE_SYSGROUP,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_SYSRUN,        WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_SYSRUNTOFRAME, WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_SYSRUNTONMI,   WM_SETFONT, (WPARAM)hFont,     TRUE);

  SendDlgItemMessage(hwnd, CONSOLE_CFGGROUP,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTCPU,  WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTAPU,  WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTDBG,  WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGTRACE,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGREGTYPE,    WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGREGNUM,     WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGREGVAL,     WM_SETFONT, (WPARAM)hMonofont, TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGREGSET,     WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCK,       WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKUP,     WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKDOWN,   WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKLEFT,   WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKRIGHT,  WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKA,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKB,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKX,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKY,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKL,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKR,      WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKSELECT, WM_SETFONT, (WPARAM)hFont,     TRUE);
  SendDlgItemMessage(hwnd, CONSOLE_CFGLOCKSTART,  WM_SETFONT, (WPARAM)hFont,     TRUE);

  outputcpu = true;
  outputapu = true;
  outputdbg = true;
  SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTCPU,  BM_SETCHECK, (WPARAM)true, 0);
  SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTAPU,  BM_SETCHECK, (WPARAM)true, 0);
  SendDlgItemMessage(hwnd, CONSOLE_CFGOUTPUTDBG,  BM_SETCHECK, (WPARAM)true, 0);

  SendDlgItemMessage(hwnd, CONSOLE_CFGREGTYPE, CB_ADDSTRING, 0, (LPARAM)"CPU");
  SendDlgItemMessage(hwnd, CONSOLE_CFGREGTYPE, CB_SETCURSEL, 0, 0);
  set_reg_list_type(REGTYPE_CPU);

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

Console::Console() {
  for(int i=0;i<CONSOLE_LINES;i++) {
    strcpy(output_line[i], "");
  }
  _is_running     = false;
  tracing_enabled = false;

  joypad_lock.up = joypad_lock.down = joypad_lock.left   = joypad_lock.right = false;
  joypad_lock.a  = joypad_lock.b    = joypad_lock.x      = joypad_lock.y     = false;
  joypad_lock.l  = joypad_lock.r    = joypad_lock.select = joypad_lock.start = false;
}
