struct MMC3 : Chip {
  MMC3(Board& board) : Chip(board) {
  }

  auto main() -> void {
    if(irqDelay) irqDelay--;
    cpu.irqLine(irqLine);
    tick();
  }

  auto irqTest(uint addr) -> void {
    if(!(chrAbus & 0x1000) && (addr & 0x1000)) {
      if(irqDelay == 0) {
        if(irqCounter == 0) {
          irqCounter = irqLatch;
        } else if(--irqCounter == 0) {
          if(irqEnable) irqLine = 1;
        }
      }
      irqDelay = 6;
    }
    chrAbus = addr;
  }

  auto addrPRG(uint addr) const -> uint {
    switch((addr >> 13) & 3) {
    case 0:
      if(prgMode == 1) return (0x3e << 13) | (addr & 0x1fff);
      return (prgBank[0] << 13) | (addr & 0x1fff);
    case 1:
      return (prgBank[1] << 13) | (addr & 0x1fff);
    case 2:
      if(prgMode == 0) return (0x3e << 13) | (addr & 0x1fff);
      return (prgBank[0] << 13) | (addr & 0x1fff);
    case 3:
      return (0x3f << 13) | (addr & 0x1fff);
    }
    unreachable;
  }

  auto addrCHR(uint addr) const -> uint {
    if(chrMode == 0) {
      if(addr <= 0x07ff) return (chrBank[0] << 10) | (addr & 0x07ff);
      if(addr <= 0x0fff) return (chrBank[1] << 10) | (addr & 0x07ff);
      if(addr <= 0x13ff) return (chrBank[2] << 10) | (addr & 0x03ff);
      if(addr <= 0x17ff) return (chrBank[3] << 10) | (addr & 0x03ff);
      if(addr <= 0x1bff) return (chrBank[4] << 10) | (addr & 0x03ff);
      if(addr <= 0x1fff) return (chrBank[5] << 10) | (addr & 0x03ff);
    } else {
      if(addr <= 0x03ff) return (chrBank[2] << 10) | (addr & 0x03ff);
      if(addr <= 0x07ff) return (chrBank[3] << 10) | (addr & 0x03ff);
      if(addr <= 0x0bff) return (chrBank[4] << 10) | (addr & 0x03ff);
      if(addr <= 0x0fff) return (chrBank[5] << 10) | (addr & 0x03ff);
      if(addr <= 0x17ff) return (chrBank[0] << 10) | (addr & 0x07ff);
      if(addr <= 0x1fff) return (chrBank[1] << 10) | (addr & 0x07ff);
    }
    return 0;
  }

  auto addrCIRAM(uint addr) const -> uint {
    if(mirror == 0) return ((addr & 0x0400) >> 0) | (addr & 0x03ff);
    if(mirror == 1) return ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    unreachable;
  }

  auto readRAM(uint addr) -> uint8 {
    if(ramEnable) return board.prgram.data[addr & 0x1fff];
    return 0x00;
  }

  auto writeRAM(uint addr, uint8 data) -> void {
    if(ramEnable && !ramWriteProtect) board.prgram.data[addr & 0x1fff] = data;
  }

  auto writeIO(uint addr, uint8 data) -> void {
    switch(addr & 0xe001) {
    case 0x8000:
      chrMode = data & 0x80;
      prgMode = data & 0x40;
      bankSelect = data & 0x07;
      break;

    case 0x8001:
      switch(bankSelect) {
      case 0: chrBank[0] = data & ~1; break;
      case 1: chrBank[1] = data & ~1; break;
      case 2: chrBank[2] = data; break;
      case 3: chrBank[3] = data; break;
      case 4: chrBank[4] = data; break;
      case 5: chrBank[5] = data; break;
      case 6: prgBank[0] = data & 0x3f; break;
      case 7: prgBank[1] = data & 0x3f; break;
      }
      break;

    case 0xa000:
      mirror = data & 0x01;
      break;

    case 0xa001:
      ramEnable = data & 0x80;
      ramWriteProtect = data & 0x40;
      break;

    case 0xc000:
      irqLatch = data;
      break;

    case 0xc001:
      irqCounter = 0;
      break;

    case 0xe000:
      irqEnable = false;
      irqLine = 0;
      break;

    case 0xe001:
      irqEnable = true;
      break;
    }
  }

  auto power() -> void {
    chrMode = 0;
    prgMode = 0;
    bankSelect = 0;
    prgBank[0] = 0;
    prgBank[1] = 0;
    chrBank[0] = 0;
    chrBank[1] = 0;
    chrBank[2] = 0;
    chrBank[3] = 0;
    chrBank[4] = 0;
    chrBank[5] = 0;
    mirror = 0;
    ramEnable = 1;
    ramWriteProtect = 0;
    irqLatch = 0;
    irqCounter = 0;
    irqEnable = false;
    irqDelay = 0;
    irqLine = 0;

    chrAbus = 0;
  }

  auto serialize(serializer& s) -> void {
    s.integer(chrMode);
    s.integer(prgMode);
    s.integer(bankSelect);
    s.array(prgBank);
    s.array(chrBank);
    s.integer(mirror);
    s.integer(ramEnable);
    s.integer(ramWriteProtect);
    s.integer(irqLatch);
    s.integer(irqCounter);
    s.integer(irqEnable);
    s.integer(irqDelay);
    s.integer(irqLine);

    s.integer(chrAbus);
  }

  bool chrMode;
  bool prgMode;
  uint3 bankSelect;
  uint8 prgBank[2];
  uint8 chrBank[6];
  bool mirror;
  bool ramEnable;
  bool ramWriteProtect;
  uint8 irqLatch;
  uint8 irqCounter;
  bool irqEnable;
  uint irqDelay;
  bool irqLine;

  uint16 chrAbus;
};
