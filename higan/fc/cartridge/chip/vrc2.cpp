struct VRC2 : Chip {
  VRC2(Board& board) : Chip(board) {
  }

  auto addrPRG(uint addr) const -> uint {
    uint bank;
    switch(addr & 0xe000) {
    case 0x8000: bank = prgBank[0]; break;
    case 0xa000: bank = prgBank[1]; break;
    case 0xc000: bank = 0x1e; break;
    case 0xe000: bank = 0x1f; break;
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

  auto readRAM(uint addr) -> uint8 {
    if(board.prgram.size == 0) {
      if((addr & 0xf000) == 0x6000) return cpu.mdr() | latch;
      return cpu.mdr();
    }
    return board.prgram.read(addr & 0x1fff);
  }

  auto writeRAM(uint addr, uint8 data) -> void {
    if(board.prgram.size == 0) {
      if((addr & 0xf000) == 0x6000) latch = data & 0x01;
      return;
    }
    return board.prgram.write(addr & 0x1fff, data);
  }

  auto writeIO(uint addr, uint8 data) -> void {
    switch(addr) {
    case 0x8000: case 0x8001: case 0x8002: case 0x8003:
      prgBank[0] = data & 0x1f;
      break;

    case 0x9000: case 0x9001: case 0x9002: case 0x9003:
      mirror = data & 0x03;
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
    }
  }

  auto power() -> void {
    for(auto& n : prgBank) n = 0;
    for(auto& n : chrBank) n = 0;
    mirror = 0;
    latch = 0;
  }

  auto serialize(serializer& s) -> void {
    for(auto& n : prgBank) s.integer(n);
    for(auto& n : chrBank) s.integer(n);
    s.integer(mirror);
    s.integer(latch);
  }

  uint5 prgBank[2];
  uint8 chrBank[8];
  uint2 mirror;
  bool latch;
};
