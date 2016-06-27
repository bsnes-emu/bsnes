struct VRC4 : Chip {
  VRC4(Board& board) : Chip(board) {
  }

  auto main() -> void {
    if(irqEnable) {
      if(irqMode == 0) {
        irqScalar -= 3;
        if(irqScalar <= 0) {
          irqScalar += 341;
          if(irqCounter == 0xff) {
            irqCounter = irqLatch;
            irqLine = 1;
          } else {
            irqCounter++;
          }
        }
      }

      if(irqMode == 1) {
        if(irqCounter == 0xff) {
          irqCounter = irqLatch;
          irqLine = 1;
        } else {
          irqCounter++;
        }
      }
    }

    cpu.irqLine(irqLine);
    tick();
  }

  auto addrPRG(uint addr) const -> uint {
    uint bank = 0, banks = board.prgrom.size / 0x2000;
    switch(addr & 0xe000) {
    case 0x8000: bank = prgMode == 0 ? (uint)prgBank[0] : banks - 2; break;
    case 0xa000: bank = prgBank[1]; break;
    case 0xc000: bank = prgMode == 0 ? banks - 2 : (uint)prgBank[0]; break;
    case 0xe000: bank = banks - 1; break;
    }
    return (bank * 0x2000) + (addr & 0x1fff);
  }

  auto addrCHR(uint addr) const -> uint {
    uint bank = chrBank[addr / 0x0400];
    return (bank * 0x0400) + (addr & 0x03ff);
  }

  auto addrCIRAM(uint addr) const -> uint {
    switch(mirror) {
    case 0: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical mirroring
    case 1: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal mirroring
    case 2: return 0x0000 | (addr & 0x03ff);                  //one-screen mirroring (first)
    case 3: return 0x0400 | (addr & 0x03ff);                  //one-screen mirroring (second)
    }
    throw;
  }

  auto writeIO(uint addr, uint8 data) -> void {
    switch(addr) {
    case 0x8000: case 0x8001: case 0x8002: case 0x8003:
      prgBank[0] = data & 0x1f;
      break;

    case 0x9000: case 0x9001:
      mirror = data & 0x03;
      break;

    case 0x9002: case 0x9003:
      prgMode = data & 0x02;
      break;

    case 0xa000: case 0xa001: case 0xa002: case 0xa003:
      prgBank[1] = data & 0x1f;
      break;

    case 0xb000: chrBank[0] = (chrBank[0] & 0xf0) | ((data & 0x0f) << 0); break;
    case 0xb001: chrBank[0] = (chrBank[0] & 0x0f) | ((data & 0x0f) << 4); break;

    case 0xb002: chrBank[1] = (chrBank[1] & 0xf0) | ((data & 0x0f) << 0); break;
    case 0xb003: chrBank[1] = (chrBank[1] & 0x0f) | ((data & 0x0f) << 4); break;

    case 0xc000: chrBank[2] = (chrBank[2] & 0xf0) | ((data & 0x0f) << 0); break;
    case 0xc001: chrBank[2] = (chrBank[2] & 0x0f) | ((data & 0x0f) << 4); break;

    case 0xc002: chrBank[3] = (chrBank[3] & 0xf0) | ((data & 0x0f) << 0); break;
    case 0xc003: chrBank[3] = (chrBank[3] & 0x0f) | ((data & 0x0f) << 4); break;

    case 0xd000: chrBank[4] = (chrBank[4] & 0xf0) | ((data & 0x0f) << 0); break;
    case 0xd001: chrBank[4] = (chrBank[4] & 0x0f) | ((data & 0x0f) << 4); break;

    case 0xd002: chrBank[5] = (chrBank[5] & 0xf0) | ((data & 0x0f) << 0); break;
    case 0xd003: chrBank[5] = (chrBank[5] & 0x0f) | ((data & 0x0f) << 4); break;

    case 0xe000: chrBank[6] = (chrBank[6] & 0xf0) | ((data & 0x0f) << 0); break;
    case 0xe001: chrBank[6] = (chrBank[6] & 0x0f) | ((data & 0x0f) << 4); break;

    case 0xe002: chrBank[7] = (chrBank[7] & 0xf0) | ((data & 0x0f) << 0); break;
    case 0xe003: chrBank[7] = (chrBank[7] & 0x0f) | ((data & 0x0f) << 4); break;

    case 0xf000:
      irqLatch = (irqLatch & 0xf0) | ((data & 0x0f) << 0);
      break;

    case 0xf001:
      irqLatch = (irqLatch & 0x0f) | ((data & 0x0f) << 4);
      break;

    case 0xf002:
      irqMode = data & 0x04;
      irqEnable = data & 0x02;
      irqAcknowledge = data & 0x01;
      if(irqEnable) {
        irqCounter = irqLatch;
        irqScalar = 341;
      }
      irqLine = 0;
      break;

    case 0xf003:
      irqEnable = irqAcknowledge;
      irqLine = 0;
      break;
    }
  }

  auto power() -> void {
  }

  auto reset() -> void {
    prgMode = 0;
    for(auto& n : prgBank) n = 0;
    mirror = 0;
    for(auto& n : chrBank) n = 0;

    irqLatch = 0;
    irqMode = 0;
    irqEnable = 0;
    irqAcknowledge = 0;

    irqCounter = 0;
    irqScalar = 0;
    irqLine = 0;
  }

  auto serialize(serializer& s) -> void {
    s.integer(prgMode);
    for(auto& n : prgBank) s.integer(n);
    s.integer(mirror);
    for(auto& n : chrBank) s.integer(n);

    s.integer(irqLatch);
    s.integer(irqMode);
    s.integer(irqEnable);
    s.integer(irqAcknowledge);

    s.integer(irqCounter);
    s.integer(irqScalar);
    s.integer(irqLine);
  }

  bool prgMode;
  uint5 prgBank[2];
  uint2 mirror;
  uint8 chrBank[8];

  uint8 irqLatch;
  bool irqMode;
  bool irqEnable;
  bool irqAcknowledge;

  uint8 irqCounter;
  int irqScalar;
  bool irqLine;
};
