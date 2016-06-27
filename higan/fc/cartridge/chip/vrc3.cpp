struct VRC3 : Chip {
  VRC3(Board& board) : Chip(board) {
  }

  auto main() -> void {
    if(irqEnable) {
      if(irqMode == 0) {  //16-bit
        if(++irqCounter.w == 0) {
          irqLine = 1;
          irqEnable = irqAcknowledge;
          irqCounter.w = irqLatch;
        }
      }
      if(irqMode == 1) {  //8-bit
        if(++irqCounter.l == 0) {
          irqLine = 1;
          irqEnable = irqAcknowledge;
          irqCounter.l = irqLatch;
        }
      }
    }

    cpu.irqLine(irqLine);
    tick();
  }

  auto addrPRG(uint addr) const -> uint {
    uint bank = (addr < 0xc000 ? (uint)prgBank : 0x0f);
    return (bank * 0x4000) + (addr & 0x3fff);
  }

  auto writeIO(uint addr, uint8 data) -> void {
    switch(addr & 0xf000) {
    case 0x8000: irqLatch = (irqLatch & 0xfff0) | ((data & 0x0f) <<  0); break;
    case 0x9000: irqLatch = (irqLatch & 0xff0f) | ((data & 0x0f) <<  4); break;
    case 0xa000: irqLatch = (irqLatch & 0xf0ff) | ((data & 0x0f) <<  8); break;
    case 0xb000: irqLatch = (irqLatch & 0x0fff) | ((data & 0x0f) << 12); break;

    case 0xc000:
      irqMode = data & 0x04;
      irqEnable = data & 0x02;
      irqAcknowledge = data & 0x01;
      if(irqEnable) irqCounter.w = irqLatch;
      break;

    case 0xd000:
      irqLine = 0;
      irqEnable = irqAcknowledge;
      break;

    case 0xf000:
      prgBank = data & 0x0f;
      break;
    }
  }

  auto power() -> void {
  }

  auto reset() -> void {
    prgBank = 0;
    irqMode = 0;
    irqEnable = 0;
    irqAcknowledge = 0;
    irqLatch = 0;
    irqCounter.w = 0;
    irqLine = 0;
  }

  auto serialize(serializer& s) -> void {
    s.integer(prgBank);
    s.integer(irqMode);
    s.integer(irqEnable);
    s.integer(irqAcknowledge);
    s.integer(irqLatch);
    s.integer(irqCounter.w);
    s.integer(irqLine);
  }

  uint4 prgBank;
  bool irqMode;
  bool irqEnable;
  bool irqAcknowledge;
  uint16 irqLatch;
  struct {
    union {
      uint16_t w;
      struct { uint8_t order_lsb2(l, h); };
    };
  } irqCounter;
  bool irqLine;
};
