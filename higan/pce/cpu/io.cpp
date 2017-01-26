auto CPU::read(uint8 bank, uint13 addr) -> uint8 {
  //$00-7f  HuCard
  if(!bank.bit(7)) {
    return cartridge.read(bank << 13 | addr);
  }

  //$f7  BRAM
  if(bank == 0xf7) {
    return bram[addr.bits(0,10)];
  }

  //$f8-fb  RAM
  if(bank >= 0xf8 && bank <= 0xfb) {
    if(Model::PCEngine()) return ram[addr];
    if(Model::SuperGrafx()) return ram[bank.bits(0,1) << 13 | addr];
  }

  //$ff  Hardware
  if(bank == 0xff) {
    //$0000-03ff  VDC or VPC
    if((addr & 0x1c00) == 0x0000) {
      if(Model::PCEngine()) return vdc0.read(addr);
      if(Model::SuperGrafx()) return vpc.read(addr);
    }

    //$0400-07ff  VCE
    if((addr & 0x1c00) == 0x0400) {
      return vce.read(addr);
    }

    //$0800-0bff  PSG
    if((addr & 0x1c00) == 0x0800) {
      return io.mdr;
    }

    //$0c00-0fff  Timer
    if((addr & 0x1c00) == 0x0c00) {
      return (io.mdr & 0x80) | timer.value;
    }

    //$1000-13ff  I/O
    if((addr & 0x1c00) == 0x1000) {
      //note 1: Turbografx-16 games check this bit for region protection.
      //yet PC Engine games do not. since we cannot tell the games apart,
      //it's more compatible to always identify as a Turbografx-16 system.
      //note 2: we state that the CD-ROM drive is present.
      //this is so games can use its backup RAM for save data.
      return (
        PCEngine::peripherals.controllerPort->readData() << 0
      | 1 << 4
      | 1 << 5
      | 0 << 6  //device (0 = Turbografx-16; 1 = PC Engine)
      | 0 << 7  //add-on (0 = CD-ROM; 1 = nothing)
      );
    }

    //$1400-17ff  IRQ
    if((addr & 0x1c00) == 0x1400) {
      if(addr.bits(0,1) == 0) {
        return io.mdr;
      }

      if(addr.bits(0,1) == 1) {
        return io.mdr;
      }

      if(addr.bits(0,1) == 2) {
        return (
          irq.disableExternal << 0
        | irq.disableVDC << 1
        | irq.disableTimer << 2
        | (io.mdr & 0xf8)
        );
      }

      if(addr.bits(0,1) == 3) {
        bool pendingExternal = 0;
        bool pendingVDC = vdc0.irqLine() | vdc1.irqLine();
        bool pendingTimer = timer.irqLine();
        return (
          pendingExternal << 0
        | pendingVDC << 1
        | pendingTimer << 2
        | (io.mdr & 0xf8)
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

  return 0xff;
}

auto CPU::write(uint8 bank, uint13 addr, uint8 data) -> void {
  //$00-7f  HuCard
  if(!bank.bit(7)) {
    return cartridge.write(bank << 13 | addr, data);
  }

  //$f7  BRAM
  if(bank == 0xf7) {
    bram[addr.bits(0,10)] = data;
    return;
  }

  //$f8-fb  RAM
  if(bank >= 0xf8 && bank <= 0xfb) {
    if(Model::PCEngine()) ram[addr] = data;
    if(Model::SuperGrafx()) ram[bank.bits(0,1) << 13 | addr] = data;
    return;
  }

  //$1fe000-1fffff  Hardware
  if(bank == 0xff) {
    //$0000-03ff  VDC or VPC
    if((addr & 0x1c00) == 0x0000) {
      if(Model::PCEngine()) return vdc0.write(addr, data);
      if(Model::SuperGrafx()) return vpc.write(addr, data);
    }

    //$0400-07ff  VCE
    if((addr & 0x1c00) == 0x0400) {
      return vce.write(addr, data);
    }

    //$0800-0bff  PSG
    if((addr & 0x1c00) == 0x0800) {
      io.mdr = data;
      return;
    }

    //$0c00-0fff  Timer
    if((addr & 0x1c00) == 0x0c00) {
      io.mdr = data;
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
      io.mdr = data;
      PCEngine::peripherals.controllerPort->writeData(data.bits(0,1));
      return;
    }

    //$1400-17ff  IRQ
    if((addr & 0x1c00) == 0x1400) {
      io.mdr = data;
      if(addr.bits(0,1) == 2) {
        irq.disableExternal = data.bit(0);
        irq.disableVDC = data.bit(1);
        irq.disableTimer = data.bit(2);
        return;
      }

      if(addr.bits(0,1) == 3) {
        timer.line = 0;
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

//ST0, ST1, ST2
auto CPU::store(uint2 addr, uint8 data) -> void {
  if(addr) addr++;  //0,1,2 => 0,2,3
  if(Model::PCEngine()) vdc0.write(addr, data);
  if(Model::SuperGrafx()) vpc.store(addr, data);
}
