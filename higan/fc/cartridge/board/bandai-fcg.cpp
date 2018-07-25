//BANDAI-FCG

struct BandaiFCG : Board {
  BandaiFCG(Markup::Node& document) : Board(document) {
  }

  auto main() -> void {
    if(irqCounterEnable) {
      if(--irqCounter == 0xffff) {
        cpu.irqLine(1);
        irqCounterEnable = false;
      }
    }

    tick();
  }

  auto addrCIRAM(uint addr) const -> uint {
    switch(mirror) {
    case 0: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);
    case 1: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    case 2: return 0x0000 | (addr & 0x03ff);
    case 3: return 0x0400 | (addr & 0x03ff);
    }
    unreachable;
  }

  auto readPRG(uint addr) -> uint8 {
    if(addr & 0x8000) {
      bool region = addr & 0x4000;
      uint bank = (region == 0 ? prgBank : (uint8)0x0f);
      return prgrom.read((bank << 14) | (addr & 0x3fff));
    }
    return cpu.mdr();
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if(addr >= 0x6000) {
      switch(addr & 15) {
      case 0x00: case 0x01: case 0x02: case 0x03:
      case 0x04: case 0x05: case 0x06: case 0x07:
        chrBank[addr & 7] = data;
        break;
      case 0x08:
        prgBank = data & 0x0f;
        break;
      case 0x09:
        mirror = data & 0x03;
        break;
      case 0x0a:
        cpu.irqLine(0);
        irqCounterEnable = data & 0x01;
        irqCounter = irqLatch;
        break;
      case 0x0b:
        irqLatch = (irqLatch & 0xff00) | (data << 0);
        break;
      case 0x0c:
        irqLatch = (irqLatch & 0x00ff) | (data << 8);
        break;
      case 0x0d:
        //todo: serial EEPROM support
        break;
      }
    }
  }

  auto readCHR(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(addrCIRAM(addr));
    addr = (chrBank[addr >> 10] << 10) | (addr & 0x03ff);
    return Board::readCHR(addr);
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(addrCIRAM(addr), data);
    addr = (chrBank[addr >> 10] << 10) | (addr & 0x03ff);
    return Board::writeCHR(addr, data);
  }

  auto power() -> void {
    reset();
  }

  auto reset() -> void {
    for(auto& n : chrBank) n = 0;
    prgBank = 0;
    mirror = 0;
    irqCounterEnable = 0;
    irqCounter = 0;
    irqLatch = 0;
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);

    s.array(chrBank);
    s.integer(prgBank);
    s.integer(mirror);
    s.integer(irqCounterEnable);
    s.integer(irqCounter);
    s.integer(irqLatch);
  }

  uint8 chrBank[8];
  uint8 prgBank;
  uint2 mirror;
  bool irqCounterEnable;
  uint16 irqCounter;
  uint16 irqLatch;
};
