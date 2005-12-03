void bSNES::power() {
  ds_sound->init();
  SNES::power();
}

void bSNES::reset() {
  SNES::reset();
}

void bSNES::set_status(uint32 new_status) {
uint8 cpu_op;
  run_status = new_status;

  switch(new_status) {
  case STOP:
    w_console->is_running(false);
    w_console->update_status();
    w_memory->refresh();
    break;
  case RUNTOCPUSTEP:
    status.cpu_ran = false;
    break;
  case RUNTOCPUPROCEED:
    cpu_op = r_mem->read(r_cpu->regs.pc.d);

    if(cpu_op == 0x10 || //bpl rel
       cpu_op == 0x30 || //bmi rel
       cpu_op == 0x50 || //bvc rel
       cpu_op == 0x70 || //bvs rel
       cpu_op == 0x90 || //bcc rel
       cpu_op == 0xb0 || //bcs rel
       cpu_op == 0xd0 || //bne rel
       cpu_op == 0xf0 || //beq rel
       cpu_op == 0x20 || //jsr addr
       cpu_op == 0x22 || //jsl long
       cpu_op == 0xfc    //jsr (addr,x)
    ) {
      w_console->is_running(true);
      status.cpu_stop_pos = (r_cpu->regs.pc.b << 16) | ((r_cpu->regs.pc.d + r_cpu->opcode_length()) & 0xffff);
    } else {
      status.cpu_ran = false;
      run_status     = RUNTOCPUSTEP;
    }
    break;
  case RUNTOAPUSTEP:
    status.apu_ran = false;
    break;
  }
}

uint32 bSNES::get_status() {
  return run_status;
}

void bSNES::run() {
  if(!r_mem->cart_loaded())return;

  switch(run_status) {
  case RUN:
    SNES::runtoframe();
    return;
  case STOP:
    break;
  case RUNONCE:
    SNES::run();
    set_status(STOP);
    break;
  case RUNTOSIGNAL:
    SNES::run();
    if(w_bp->hit() == true) {
      set_status(STOP);
      disassemble_bp_op();
    }
    break;
  case RUNTOFRAME:
    SNES::run();
    if(r_ppu->status.frame_executed == true) {
      r_ppu->status.frame_executed = false;
      set_status(STOP);
      disassemble_apu_op();
      disassemble_cpu_op();
    } else if(w_bp->hit() == true) {
      set_status(STOP);
      disassemble_bp_op();
    }
    return;
  case RUNTOCPUSTEP:
    SNES::run();
    if(status.cpu_ran == true) {
      set_status(STOP);
    } else if(w_bp->hit() == true) {
      set_status(STOP);
      disassemble_bp_op();
    }
    break;
  case RUNTOCPUPROCEED:
    SNES::run();
    if(r_cpu->in_opcode() == false && status.cpu_stop_pos == r_cpu->regs.pc.d) {
      set_status(STOP);
      disassemble_cpu_op();
    } else if(w_bp->hit() == true) {
      set_status(STOP);
      disassemble_bp_op();
    }
    break;
  case RUNTOCPUTRACE:
    SNES::run();
    if(status.cpu_trace_pos >= status.cpu_trace_stop) {
      set_status(STOP);
      disassemble_cpu_op();
    }
    break;
  case RUNTOAPUSTEP:
    SNES::run();
    if(status.apu_ran == true || w_bp->hit() == true) {
      set_status(STOP);
    }
    break;
  }
}

void bSNES::video_run() {
  if(r_ppu->status.frames_updated) {
  char s[512], t[512];
    r_ppu->status.frames_updated = false;
    if((bool)config::gui.show_fps == true) {
      sprintf(s, "%s : %d fps", BSNES_TITLE, r_ppu->status.frames_executed);
      if(w_main->frameskip != 0) {
        sprintf(t, " (%d frames)", r_ppu->status.frames_rendered);
        strcat(s, t);
      }
      SetWindowText(w_main->hwnd, s);
    }
  }

  w_main->frameskip_pos++;
  w_main->frameskip_pos %= (w_main->frameskip + 1);
  if(r_ppu->renderer_enabled())dd_renderer->update();
  r_ppu->enable_renderer(w_main->frameskip_pos == 0);
}

void bSNES::sound_run(uint32 data) {
  ds_sound->run(data);
}

/***********************
 *** Video functions ***
 ***********************/
uint16 *bSNES::video_lock(uint32 &pitch) {
  return dd_renderer->lock(pitch);
}

void bSNES::video_unlock() {
  dd_renderer->unlock();
}

/***********************
 *** Input functions ***
 ***********************/
void bSNES::clear_input() {
  joypad1.up     = joypad2.up =
  joypad1.down   = joypad2.down =
  joypad1.left   = joypad2.left =
  joypad1.right  = joypad2.right =
  joypad1.a      = joypad2.a =
  joypad1.b      = joypad2.b =
  joypad1.x      = joypad2.x =
  joypad1.y      = joypad2.y =
  joypad1.l      = joypad2.l =
  joypad1.r      = joypad2.r =
  joypad1.select = joypad2.select =
  joypad1.start  = joypad2.start = 0;
}

void bSNES::poll_input(uint8 type) {
//only capture input when main window has focus
  if(GetForegroundWindow() == w_main->hwnd) {
    switch(type) {
    case SNES::DEV_JOYPAD1:
      joypad1.up     = KeyDown(config::input.joypad1.up);
      joypad1.down   = KeyDown(config::input.joypad1.down);
      joypad1.left   = KeyDown(config::input.joypad1.left);
      joypad1.right  = KeyDown(config::input.joypad1.right);
      joypad1.select = KeyDown(config::input.joypad1.select);
      joypad1.start  = KeyDown(config::input.joypad1.start);
      joypad1.y      = KeyDown(config::input.joypad1.y);
      joypad1.b      = KeyDown(config::input.joypad1.b);
      joypad1.x      = KeyDown(config::input.joypad1.x);
      joypad1.a      = KeyDown(config::input.joypad1.a);
      joypad1.l      = KeyDown(config::input.joypad1.l);
      joypad1.r      = KeyDown(config::input.joypad1.r);
      break;
    case SNES::DEV_JOYPAD2:
      joypad2.up     = KeyDown(config::input.joypad2.up);
      joypad2.down   = KeyDown(config::input.joypad2.down);
      joypad2.left   = KeyDown(config::input.joypad2.left);
      joypad2.right  = KeyDown(config::input.joypad2.right);
      joypad2.select = KeyDown(config::input.joypad2.select);
      joypad2.start  = KeyDown(config::input.joypad2.start);
      joypad2.y      = KeyDown(config::input.joypad2.y);
      joypad2.b      = KeyDown(config::input.joypad2.b);
      joypad2.x      = KeyDown(config::input.joypad2.x);
      joypad2.a      = KeyDown(config::input.joypad2.a);
      joypad2.l      = KeyDown(config::input.joypad2.l);
      joypad2.r      = KeyDown(config::input.joypad2.r);
      break;
    }
  } else {
    switch(type) {
    case SNES::DEV_JOYPAD1:
      joypad1.up = joypad1.down = joypad1.left = joypad1.right =
      joypad1.select = joypad1.start =
      joypad1.y = joypad1.b = joypad1.x = joypad1.a =
      joypad1.l = joypad1.r = 0;
      break;
    case SNES::DEV_JOYPAD2:
      joypad2.up = joypad2.down = joypad2.left = joypad2.right =
      joypad2.select = joypad2.start =
      joypad2.y = joypad2.b = joypad2.x = joypad2.a =
      joypad1.l = joypad2.r = 0;
      break;
    }
  }

//check for debugger-based key locks
  if(is_debugger_enabled == true && type == SNES::DEV_JOYPAD1) {
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

//it's impossible to hold both up+down, or left+right down
//at the same time on a directional pad; and besides, allowing
//this to happen causes glitches in many SNES games.
  if(joypad1.up)  joypad1.down  = 0;
  if(joypad1.left)joypad1.right = 0;

  if(joypad2.up)  joypad2.down  = 0;
  if(joypad2.left)joypad2.right = 0;
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
  case DEV_JOYPAD2:
    switch(button) {
    case JOYPAD_UP:    return joypad2.up;
    case JOYPAD_DOWN:  return joypad2.down;
    case JOYPAD_LEFT:  return joypad2.left;
    case JOYPAD_RIGHT: return joypad2.right;
    case JOYPAD_A:     return joypad2.a;
    case JOYPAD_B:     return joypad2.b;
    case JOYPAD_X:     return joypad2.x;
    case JOYPAD_Y:     return joypad2.y;
    case JOYPAD_L:     return joypad2.l;
    case JOYPAD_R:     return joypad2.r;
    case JOYPAD_SELECT:return joypad2.select;
    case JOYPAD_START: return joypad2.start;
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
    //don't let the debugger poll MMIO ports
      if(a >= 0x2000 && a <= 0x5fff) {
        r = 0x00;
      } else {
        r = r_mem->read(addr);
      }
    } else {
      r = r_mem->read(addr);
    }
    break;
  case SPCRAM:
    addr &= 0xffff;
    r = r_apu->spcram_read(addr);
    break;
  case VRAM:
    addr &= 0xffff;
    r = r_ppu->vram_read(addr);
    break;
  case OAM:
    addr &= 0x03ff;
    r = r_ppu->oam_read(addr);
    break;
  case CGRAM:
    addr &= 0x01ff;
    r = r_ppu->cgram_read(addr);
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
    r_mem->cart->write_protect(false);
    r_mem->write(addr, value);
    r_mem->cart->write_protect(true);
    break;
  case SPCRAM:
    addr &= 0xffff;
    r_apu->spcram_write(addr, value);
    break;
  case VRAM:
    addr &= 0xffff;
    r_ppu->vram_write(addr, value);
    break;
  case OAM:
    addr &= 0x03ff;
    r_ppu->oam_write(addr, value);
    break;
  case CGRAM:
    addr &= 0x01ff;
    r_ppu->cgram_write(addr, value);
    break;
  }
  debug_command = false;
}

void bSNES::notify(uint32 message, uint32 param1, uint32 param2) {
//debugging messages
  if(is_debugger_enabled == false)return;

  switch(message) {
  case CPU_EXEC_OPCODE_BEGIN:
    break;
  case CPU_EXEC_OPCODE_END:
    status.cpu_ran = true;
    status.cpu_trace_pos++;
  //test next opcode for breakpoint
    w_bp->test(message, r_cpu->regs.pc.d, 0);
    disassemble_cpu_op();
    break;
  case APU_EXEC_OPCODE_BEGIN:
    break;
  case APU_EXEC_OPCODE_END:
    status.apu_ran = true;
    w_bp->test(message, r_apu->regs.pc, 0);
    disassemble_apu_op();
    break;
  case MEM_READ:
  case SPCRAM_READ:
  case VRAM_READ:
  case OAM_READ:
  case CGRAM_READ:
    if(debug_command == false) {
      w_bp->test(message, param1, param2);
    }
    break;
  case MEM_WRITE:
  case SPCRAM_WRITE:
  case VRAM_WRITE:
  case OAM_WRITE:
  case CGRAM_WRITE:
    if(debug_command == false) {
      w_bp->test(message, param1, param2);
    }
  //this needs to be called after the breakpoint test,
  //as it will access read() and clear debug_command
    w_memory->refresh(message, param1);
    break;
  }
}

void bSNES::disassemble_cpu_op() {
char t[512];
//don't disassemble opcodes when no ROM is loaded
  if(is_debugger_activated == false)return;

//don't disassemble opcodes that won't be printed to console/traced to log anyway
  if(!w_console->can_write(Console::CPU_MESSAGE) && !w_console->tracing_enabled)return;

  r_cpu->disassemble_opcode(t);
  w_console->write(t, Console::CPU_MESSAGE);
}

void bSNES::disassemble_apu_op() {
char t[512];
  if(is_debugger_activated == false)return;

  if(!w_console->can_write(Console::APU_MESSAGE) && !w_console->tracing_enabled)return;

  r_apu->disassemble_opcode(t);
  w_console->write(t, Console::APU_MESSAGE);
}

void bSNES::disassemble_bp_op() {
  if(w_bp->bp[w_bp->bp_hit_num].source == SPCRAM) {
    disassemble_apu_op();
  } else {
    disassemble_cpu_op();
  }
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
  disassemble_apu_op();
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

  debugger_disable();
}
