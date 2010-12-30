#ifdef CPU_CPP

uint8 CPU::mmio_read(uint16 addr) {
  if(addr >= 0xc000 && addr <= 0xdfff) return wram[addr & 0x1fff];
  if(addr >= 0xe000 && addr <= 0xfdff) return wram[addr & 0x1fff];
  if(addr >= 0xff80 && addr <= 0xfffe) return hram[addr & 0x7f];

  if(addr == 0xff00) {  //JOYP
    unsigned keys = 0x0f;

    if(status.p15 == 0 && status.p14 == 1) {
      keys  = !system.interface->input_poll((unsigned)Input::Down)   << 3;
      keys |= !system.interface->input_poll((unsigned)Input::Up)     << 2;
      keys |= !system.interface->input_poll((unsigned)Input::Left)   << 1;
      keys |= !system.interface->input_poll((unsigned)Input::Right)  << 0;
    }

    if(status.p15 == 1 && status.p14 == 0) {
      keys  = !system.interface->input_poll((unsigned)Input::Start)  << 3;
      keys |= !system.interface->input_poll((unsigned)Input::Select) << 2;
      keys |= !system.interface->input_poll((unsigned)Input::B)      << 1;
      keys |= !system.interface->input_poll((unsigned)Input::A)      << 0;
    }

    return (status.p15 << 5)
         | (status.p14 << 4)
         | (keys << 0);
  }

  if(addr == 0xff04) {  //DIV
    return status.div;
  }

  if(addr == 0xff05) {  //TIMA
    return status.tima;
  }

  if(addr == 0xff06) {  //TMA
    return status.tma;
  }

  if(addr == 0xff07) {  //TAC
    return (status.timer_enable << 2)
         | (status.timer_clock << 0);
  }

  if(addr == 0xff0f) {  //IF
    return (status.interrupt_request_joypad << 4)
         | (status.interrupt_request_serial << 3)
         | (status.interrupt_request_timer << 2)
         | (status.interrupt_request_stat << 1)
         | (status.interrupt_request_vblank << 0);
  }

  if(addr == 0xffff) {  //IE
    return (status.interrupt_enable_joypad << 4)
         | (status.interrupt_enable_serial << 3)
         | (status.interrupt_enable_timer << 2)
         | (status.interrupt_enable_stat << 1)
         | (status.interrupt_enable_vblank << 0);
  }

  return 0x00;
}

void CPU::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0xc000 && addr <= 0xdfff) { wram[addr & 0x1fff] = data; return; }
  if(addr >= 0xe000 && addr <= 0xfdff) { wram[addr & 0x1fff] = data; return; }
  if(addr >= 0xff80 && addr <= 0xfffe) { hram[addr & 0x7f] = data; return; }

  if(addr == 0xff00) {  //JOYP
    status.p15 = data & 0x20;
    status.p14 = data & 0x10;
    return;
  }

  if(addr == 0xff01) {  //SB
    return;
  }

  if(addr == 0xff02) {  //SC
    return;
  }

  if(addr == 0xff04) {  //DIV
    status.div = 0;
    return;
  }

  if(addr == 0xff05) {  //TIMA
    status.tima = data;
    return;
  }

  if(addr == 0xff06) {  //TMA
    status.tma = data;
    return;
  }

  if(addr == 0xff07) {  //TAC
    status.timer_enable = data & 0x04;
    status.timer_clock = data & 0x03;
    return;
  }

  if(addr == 0xff0f) {  //IF
    status.interrupt_request_joypad = data & 0x10;
    status.interrupt_request_serial = data & 0x08;
    status.interrupt_request_timer = data & 0x04;
    status.interrupt_request_stat = data & 0x02;
    status.interrupt_request_vblank = data & 0x01;
    return;
  }

  if(addr == 0xffff) {  //IE
    status.interrupt_enable_joypad = data & 0x10;
    status.interrupt_enable_serial = data & 0x08;
    status.interrupt_enable_timer = data & 0x04;
    status.interrupt_enable_stat = data & 0x02;
    status.interrupt_enable_vblank = data & 0x01;
    return;
  }
}

#endif
