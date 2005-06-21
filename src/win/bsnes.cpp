void bSNES::set_status(uint32 new_status) {
uint8 cpu_op;
  run_status = new_status;

  switch(new_status) {
  case STOP:
    w_console->is_running(false);
    w_console->update_status();
    break;
  case RUNTOCPUSTEP:
    status.cpu_ran = false;
    break;
  case RUNTOCPUPROCEED:
    cpu_op = mem_bus->read(cpu->regs.pc);
  /* proceed only skips subroutines (jsr <addr>). 0x20, 0x22, and 0xfc are all three jsr opcodes */
    if(cpu_op == 0x20 || cpu_op == 0x22 || cpu_op == 0xfc) {
      w_console->is_running(true);
      status.cpu_stop_pos = (cpu->regs.pc.b << 16) | ((cpu->regs.pc + cpu->opcode_length()) & 0xffff);
    } else {
      status.cpu_ran = false;
      run_status     = RUNTOCPUSTEP;
    }
    break;
  }
}

uint32 bSNES::get_status() {
  return run_status;
}

void bSNES::run() {
  if(!rom_image->loaded())return;

  switch(run_status) {
  case RUN:
    while(update_frame == false) {
      clock->run();
    }
    update_frame = false;
    return;
  case STOP:
    break;
  case RUNONCE:
    clock->run();
    set_status(STOP);
    break;
  case RUNTOSIGNAL:
    clock->run();
    if(w_bp->hit() == true) {
      set_status(STOP);
      disassemble_cpu_op();
    }
    break;
  case RUNTOFRAME:
    clock->run();
    if(update_frame == true) {
      update_frame = false;
      set_status(STOP);
      disassemble_cpu_op();
    } else if(w_bp->hit() == true) {
      set_status(STOP);
      disassemble_cpu_op();
    }
    return;
  case RUNTOCPUSTEP:
    clock->run();
    if(status.cpu_ran == true || w_bp->hit() == true) {
      set_status(STOP);
    }
    break;
  case RUNTOCPUPROCEED:
    clock->run();
    if(status.cpu_stop_pos == cpu->regs.pc || w_bp->hit() == true) {
      set_status(STOP);
      disassemble_cpu_op();
    }
    break;
  case RUNTOCPUTRACE:
    clock->run();
    if(status.cpu_trace_pos >= status.cpu_trace_stop) {
      set_status(STOP);
      disassemble_cpu_op();
    }
    break;
  }
}

void bSNES::render_frame() {
  dd_renderer->update();
}

/***********************
 *** Input functions ***
 ***********************/
#define KeyState(key) ((GetAsyncKeyState(key) & 0x8000)?1:0)
void bSNES::poll_input() {
/* Only capture input when main window has focus */
  if(GetForegroundWindow() == w_main->hwnd) {
    joypad1.up     = KeyState(VK_UP);
    joypad1.down   = KeyState(VK_DOWN);
    joypad1.left   = KeyState(VK_LEFT);
    joypad1.right  = KeyState(VK_RIGHT);
    joypad1.select = KeyState(VK_LSHIFT) | KeyState(VK_RSHIFT);
    joypad1.start  = KeyState(VK_RETURN);
    joypad1.y      = KeyState('A');
    joypad1.b      = KeyState('Z');
    joypad1.x      = KeyState('S');
    joypad1.a      = KeyState('X');
    joypad1.l      = KeyState('D');
    joypad1.r      = KeyState('C');
  }

//Check for debugger-based key locks
  if(is_debugger_enabled == true) {
    if(w_console->joypad_lock.up    )joypad1.up     = true;
    if(w_console->joypad_lock.down  )joypad1.down   = true;
    if(w_console->joypad_lock.left  )joypad1.left   = true;
    if(w_console->joypad_lock.right )joypad1.right  = true;
    if(w_console->joypad_lock.a     )joypad1.a      = true;
    if(w_console->joypad_lock.b     )joypad1.b      = true;
    if(w_console->joypad_lock.x     )joypad1.x      = true;
    if(w_console->joypad_lock.y     )joypad1.y      = true;
    if(w_console->joypad_lock.l     )joypad1.l      = true;
    if(w_console->joypad_lock.r     )joypad1.r      = true;
    if(w_console->joypad_lock.select)joypad1.select = true;
    if(w_console->joypad_lock.start )joypad1.start  = true;
  }

//It's impossible to hold both up+down, or left+right down
//at the same time on a directional pad; and besides, allowing
//this to happen causes glitches in many SNES games.
  if(joypad1.up)  joypad1.down  = 0;
  if(joypad1.left)joypad1.right = 0;
}

bool bSNES::get_input_status(uint8 device, uint8 button) {
  switch(device) {
  case DEV_JOYPAD1:
    switch(button) {
    case JOYPAD_UP:    return joypad1.up;
    case JOYPAD_DOWN:  return joypad1.down;
    case JOYPAD_LEFT:  return joypad1.left;
    case JOYPAD_RIGHT: return joypad1.right;
    case JOYPAD_A:     return joypad1.a;
    case JOYPAD_B:     return joypad1.b;
    case JOYPAD_X:     return joypad1.x;
    case JOYPAD_Y:     return joypad1.y;
    case JOYPAD_L:     return joypad1.l;
    case JOYPAD_R:     return joypad1.r;
    case JOYPAD_SELECT:return joypad1.select;
    case JOYPAD_START: return joypad1.start;
    }
    break;
  }
  return 0;
}

bJoypad::bJoypad() {
  up = down = left = right = false;
  a = b = x = y = false;
  l = r = select = start = false;
}

/***************************
 *** Debugging functions ***
 ***************************/
uint8 bSNES::read(uint8 type, uint32 addr) {
uint32 bank, a;
uint8  r = 0x00;
  debug_command = true;
  switch(type) {
  case DRAM:
    addr &= 0xffffff;
    bank = (addr >> 16) & 0xff;
    a = addr & 0xffff;
    if((bank >= 0x00 && bank <= 0x3f) ||
       (bank >= 0x80 && bank <= 0xbf)) {
    /* Don't let the debugger poll MMIO ports */
      if(a >= 0x2000 && a <= 0x5fff) {
        r = 0x00;
      } else {
        r = mem_bus->read(addr);
      }
    } else {
      r = mem_bus->read(addr);
    }
    break;
  case VRAM:
    addr &= 0xffff;
    r = ppu->vram_read(addr);
    break;
  case OAM:
    addr &= 0x03ff;
    r = ppu->oam_read(addr);
    break;
  case CGRAM:
    addr &= 0x01ff;
    r = ppu->cgram_read(addr);
    break;
  }
  debug_command = false;
  return r;
}

void bSNES::write(uint8 type, uint32 addr, uint8 value) {
  debug_command = true;
  switch(type) {
  case DRAM:
    addr &= 0xffffff;
    mem_bus->rom->write_protect(false);
    mem_bus->write(addr, value);
    mem_bus->rom->write_protect(true);
    break;
  case VRAM:
    addr &= 0xffff;
    ppu->vram_write(addr, value);
    break;
  case OAM:
    addr &= 0x03ff;
    ppu->oam_write(addr, value);
    break;
  case CGRAM:
    addr &= 0x01ff;
    ppu->cgram_write(addr, value);
    break;
  }
  debug_command = false;
}

void bSNES::notify(uint32 message, uint32 param1, uint32 param2) {
/* system messages */
  switch(message) {
  case RENDER_FRAME:
    update_frame = true;
    render_frame();
    break;
  }

/* debugging messages */
  if(is_debugger_enabled == false)return;

  switch(message) {
  case CPU_EXEC_OPCODE:
    status.cpu_ran = true;
    status.cpu_trace_pos++;
    w_bp->test(message, cpu->regs.pc.d, 0);
    disassemble_cpu_op();
    break;
  case MEM_READ:
  case VRAM_READ:
  case OAM_READ:
  case CGRAM_READ:
    if(debug_command == false) {
      w_bp->test(message, param1, param2);
    }
    break;
  case MEM_WRITE:
  case VRAM_WRITE:
  case OAM_WRITE:
  case CGRAM_WRITE:
    if(debug_command == false) {
      w_bp->test(message, param1, param2);
    }
  /* this needs to be called after the breakpoint test,
     as it will access read() and clear debug_command */
    w_memory->refresh(message, param1);
    break;
  }
}

void bSNES::disassemble_cpu_op() {
char t[512];
/* don't disassemble opcodes when no ROM is loaded */
  if(is_debugger_activated == false)return;

/* don't disassemble opcodes that won't be printed to console/traced to log anyway */
  if(!w_console->can_write(Console::CPU_MESSAGE) && !w_console->tracing_enabled)return;

  cpu->disassemble_opcode(t);
  w_console->write(t, Console::CPU_MESSAGE);
}

bool bSNES::debugger_enabled() {
  return is_debugger_enabled;
}

void bSNES::debugger_enable() {
  CheckMenuItem(w_main->hmenu, MENU_SETTINGS_DEBUGGER, MF_CHECKED);
  set_status(bSNES::STOP);

  w_console->is_running(false);
  w_console->update_status();
  w_console->show();

  w_bp->refresh();
  w_bp->show();

  w_memory->edit_mode = MemoryEditor::MODE_DRAM;
  w_memory->edit_addr = 0x7e0000;
  w_memory->edit_mask = 0xffffff;
  SendDlgItemMessage(w_memory->hwnd, MEMORYEDITOR_MODE, CB_SETCURSEL, 0, 0);
  w_memory->refresh();
  w_memory->show();

  debugger_update();
  is_debugger_enabled = true;
}

void bSNES::debugger_disable() {
  CheckMenuItem(w_main->hmenu, MENU_SETTINGS_DEBUGGER, MF_UNCHECKED);
  w_console->hide();
  w_bp->hide();
  w_memory->hide();
  is_debugger_enabled = false;
  set_status(bSNES::RUN);
}

void bSNES::debugger_update() {
  w_console->update_status();
  w_bp->refresh();
  w_memory->refresh();
  disassemble_cpu_op();
}

bool bSNES::debugger_activated() {
  return is_debugger_activated;
}

void bSNES::debugger_activate() {
HWND hwnd;
uint32 i, style;
  for(i=0;i<100;i++) {
    if(w_console->ctl_disabled[i] == true)continue;
    hwnd = GetDlgItem(w_console->hwnd, i + 100);
    if(!hwnd)continue;
    style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_DISABLED;
    SetWindowLong(hwnd, GWL_STYLE, style);
  }
  InvalidateRect(w_console->hwnd, 0, TRUE);

  for(i=0;i<100;i++) {
    if(w_bp->ctl_disabled[i] == true)continue;
    hwnd = GetDlgItem(w_bp->hwnd, i + 100);
    if(!hwnd)continue;
    style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_DISABLED;
    SetWindowLong(hwnd, GWL_STYLE, style);
  }
  InvalidateRect(w_bp->hwnd, 0, TRUE);

  for(i=0;i<100;i++) {
    if(w_memory->ctl_disabled[i] == true)continue;
    hwnd = GetDlgItem(w_memory->hwnd, i + 100);
    if(!hwnd)continue;
    style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_DISABLED;
    SetWindowLong(hwnd, GWL_STYLE, style);
  }
  InvalidateRect(w_memory->hwnd, 0, TRUE);

  is_debugger_activated = true;
}

void bSNES::debugger_deactivate() {
HWND hwnd;
uint32 i, style;
  for(i=0;i<100;i++) {
    hwnd = GetDlgItem(w_console->hwnd, i + 100);
    if(!hwnd)continue;
    style = GetWindowLong(hwnd, GWL_STYLE);
    style |= WS_DISABLED;
    SetWindowLong(hwnd, GWL_STYLE, style);
  }
  w_console->clear();
  InvalidateRect(w_console->hwnd, 0, TRUE);

  for(i=0;i<100;i++) {
    hwnd = GetDlgItem(w_bp->hwnd, i + 100);
    if(!hwnd)continue;
    style = GetWindowLong(hwnd, GWL_STYLE);
    style |= WS_DISABLED;
    SetWindowLong(hwnd, GWL_STYLE, style);
  }
  w_bp->clear();
  InvalidateRect(w_bp->hwnd, 0, TRUE);


  for(i=0;i<100;i++) {
    hwnd = GetDlgItem(w_memory->hwnd, i + 100);
    if(!hwnd)continue;
    style = GetWindowLong(hwnd, GWL_STYLE);
    style |= WS_DISABLED;
    SetWindowLong(hwnd, GWL_STYLE, style);
  }
  w_memory->clear();
  InvalidateRect(w_memory->hwnd, 0, TRUE);

  is_debugger_activated = false;
}

bSNES::bSNES() {
  run_status    = STOP;
  debug_command = false;
  update_frame  = false;

  debugger_disable();
}
