auto CPU::read(uint21 addr) -> uint8 {
  //$000000-0fffff  HuCard
  if(!addr.bit(20)) {
    return cartridge.read(addr);
  }

  uint8 bank = addr.bits(13,20);
  addr = addr.bits(0,12);

  //$1f8000-1fbfff  RAM
  if(bank >= 0xf8 && bank <= 0xfb) {
    return ram[addr];
  }

  //$1fe000-$1fffff  Hardware
  if(bank == 0xff) {
    //$0000-03ff  VDC
    //$0400-07ff  VCE
    if((addr & 0x1800) == 0x0000) {
      return vdc.read(addr);
    }

    //$0800-0bff  PSG
    if((addr & 0x1c00) == 0x0800) {
      return 0x00;
    }

    //$0c00-0fff  Timer
    if((addr & 0x1c00) == 0x0c00) {
      return timer.value;
    }

    //$1000-13ff  I/O
    if((addr & 0x1c00) == 0x1000) {
      return (
        PCEngine::peripherals.controllerPort->readData() << 0
      | 1 << 4
      | 1 << 5
      | 0 << 6  //device (0 = Turbografx-16; 1 = PC Engine)
      | 1 << 7  //add-on (0 = CD-ROM; 1 = nothing)
      );
    }

    //$1400-17ff  IRQ
    if((addr & 0x1c00) == 0x1400) {
      if(addr.bits(0,1) == 2) {
        return (
          irq.disableExternal << 0
        | irq.disableVDC << 1
        | irq.disableTimer << 2
        );
      }

      if(addr.bits(0,1) == 3) {
        return (
          irq.pendingExternal << 0
        | irq.pendingVDC << 1
        | irq.pendingTimer << 2
        );
      }
    }

    //$1800-1bff  CD-ROM
    if((addr & 0x1c00) == 0x1800) {
      return 0xff;
    }

    //$1c00-1fff  unmapped
    if((addr & 0x1c00) == 0x1c00) {
      return 0xff;
    }
  }

  return 0x00;
}

auto CPU::write(uint21 addr, uint8 data) -> void {
  //$000000-0fffff  HuCard
  if(!addr.bit(20)) {
    return cartridge.write(addr, data);
  }

  uint8 bank = addr.bits(13,20);
  addr = addr.bits(0,12);

  //$1f8000-1fbfff  RAM
  if(bank >= 0xf8 && bank <= 0xfb) {
    ram[addr] = data;
    return;
  }

  //$1fe000-1fffff  Hardware
  if(bank == 0xff) {
    //$0000-03ff  VDC
    //$0400-07ff  VCE
    if((addr & 0x1800) == 0x0000) {
      return vdc.write(addr, data);
    }

    //$0800-0bff  PSG
    if((addr & 0x1c00) == 0x0800) {
      return;
    }

    //$0c00-0fff  Timer
    if((addr & 0x1c00) == 0x0c00) {
      if(!addr.bit(0)) {
        timer.latch = data.bits(0,6);
      } else {
        timer.enable = data.bit(0);
        if(timer.enable) timer.start();
      }
      return;
    }

    //$1000-13ff  I/O
    if((addr & 0x1c00) == 0x1000) {
      PCEngine::peripherals.controllerPort->writeData(data.bits(0,1));
      return;
    }

    //$1400-17ff  IRQ
    if((addr & 0x1c00) == 0x1400) {
      if(addr.bits(0,1) == 2) {
        irq.disableExternal = data.bit(0);
        irq.disableVDC = data.bit(1);
        irq.disableTimer = data.bit(2);
        return;
      }

      if(addr.bits(0,1) == 3) {
        irq.level(IRQ::Line::Timer, 0);
        return;
      }
    }

    //$1800-1bff  CD-ROM
    if((addr & 0x1c00) == 0x1800) {
      return;
    }

    //$1c00-1fff  unmapped
    if((addr & 0x1c00) == 0x1c00) {
      return;
    }
  }
}
