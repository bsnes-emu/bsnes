//Konami VRC7
//Yamaha YM2413 OPLL audio - not emulated

struct VRC7 : Chip {
  VRC7(Board& board) : Chip(board) {
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

  auto writeIO(uint addr, uint8 data) -> void {
    switch(addr) {
    case 0x8000: prgBank[0] = data; break;
    case 0x8010: prgBank[1] = data; break;
    case 0x9000: prgBank[2] = data; break;
    case 0x9010: break;  //APU addr port
    case 0x9030: break;  //APU data port
    case 0xa000: chrBank[0] = data; break;
    case 0xa010: chrBank[1] = data; break;
    case 0xb000: chrBank[2] = data; break;
    case 0xb010: chrBank[3] = data; break;
    case 0xc000: chrBank[4] = data; break;
    case 0xc010: chrBank[5] = data; break;
    case 0xd000: chrBank[6] = data; break;
    case 0xd010: chrBank[7] = data; break;
    case 0xe000: mirror = data & 0x03; break;

    case 0xe010:
      irqLatch = data;
      break;

    case 0xf000:
      irqMode = data & 0x04;
      irqEnable = data & 0x02;
      irqAcknowledge = data & 0x01;
      if(irqEnable) {
        irqCounter = irqLatch;
        irqScalar = 341;
      }
      irqLine = 0;
      break;

    case 0xf010:
      irqEnable = irqAcknowledge;
      irqLine = 0;
      break;
    }
  }

  auto addrPRG(uint addr) const -> uint {
    uint bank = 0;
    switch(addr & 0xe000) {
    case 0x8000: bank = prgBank[0]; break;
    case 0xa000: bank = prgBank[1]; break;
    case 0xc000: bank = prgBank[2]; break;
    case 0xe000: bank = 0xff; break;
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
  }

  auto power() -> void {
  }

  auto reset() -> void {
    for(auto& n : prgBank) n = 0;
    for(auto& n : chrBank) n = 0;
    mirror = 0;

    irqLatch = 0;
    irqMode = 0;
    irqEnable = 0;
    irqAcknowledge = 0;

    irqCounter = 0;
    irqScalar = 0;
    irqLine = 0;
  }

  auto serialize(serializer& s) -> void {
    s.array(prgBank);
    s.array(chrBank);
    s.integer(mirror);

    s.integer(irqLatch);
    s.integer(irqMode);
    s.integer(irqEnable);
    s.integer(irqAcknowledge);

    s.integer(irqCounter);
    s.integer(irqScalar);
    s.integer(irqLine);
  }

  uint8 prgBank[3];
  uint8 chrBank[8];
  uint2 mirror;

  uint8 irqLatch;
  bool irqMode;
  bool irqEnable;
  bool irqAcknowledge;

  uint8 irqCounter;
  int irqScalar;
  bool irqLine;
};
