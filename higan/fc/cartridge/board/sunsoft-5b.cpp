//SUNSOFT-5B

struct Sunsoft5B : Board {
  Sunsoft5B(Markup::Node& document) : Board(document) {
  }

  struct Pulse {
    auto clock() -> void {
      if(--counter == 0) {
        counter = frequency << 4;
        duty ^= 1;
      }
      output = duty ? volume : (uint4)0;
      if(disable) output = 0;
    }

    auto power() -> void {
      disable = 1;
      frequency = 1;
      volume = 0;

      counter = 0;
      duty = 0;
      output = 0;
    }

    auto serialize(serializer& s) -> void {
      s.integer(disable);
      s.integer(frequency);
      s.integer(volume);

      s.integer(counter);
      s.integer(duty);
      s.integer(output);
    }

    bool disable;
    uint12 frequency;
    uint4 volume;

    uint16 counter;  //12-bit countdown + 4-bit phase
    uint1 duty;
    uint4 output;
  } pulse[3];

  auto main() -> void {
    if(irqCounterEnable) {
      if(--irqCounter == 0xffff) {
        cpu.irqLine(irqEnable);
      }
    }

    pulse[0].clock();
    pulse[1].clock();
    pulse[2].clock();
    int16 output = dac[pulse[0].output] + dac[pulse[1].output] + dac[pulse[2].output];
    apu.setSample(-output);

    tick();
  }

  auto readPRG(uint addr) -> uint8 {
    if(addr < 0x6000) return cpu.mdr();

    uint8 bank = 0x3f;  //((addr & 0xe000) == 0xe000
    if((addr & 0xe000) == 0x6000) bank = prgBank[0];
    if((addr & 0xe000) == 0x8000) bank = prgBank[1];
    if((addr & 0xe000) == 0xa000) bank = prgBank[2];
    if((addr & 0xe000) == 0xc000) bank = prgBank[3];

    bool ramEnable = bank & 0x80;
    bool ramSelect = bank & 0x40;
    bank &= 0x3f;

    if(ramSelect) {
      if(!ramEnable) return cpu.mdr();
      return prgram.data[addr & 0x1fff];
    }

    addr = (bank << 13) | (addr & 0x1fff);
    return prgrom.read(addr);
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if((addr & 0xe000) == 0x6000) {
      prgram.data[addr & 0x1fff] = data;
    }

    if(addr == 0x8000) {
      mmuPort = data & 0x0f;
    }

    if(addr == 0xa000) {
      switch(mmuPort) {
      case  0: chrBank[0] = data; break;
      case  1: chrBank[1] = data; break;
      case  2: chrBank[2] = data; break;
      case  3: chrBank[3] = data; break;
      case  4: chrBank[4] = data; break;
      case  5: chrBank[5] = data; break;
      case  6: chrBank[6] = data; break;
      case  7: chrBank[7] = data; break;
      case  8: prgBank[0] = data; break;
      case  9: prgBank[1] = data; break;
      case 10: prgBank[2] = data; break;
      case 11: prgBank[3] = data; break;
      case 12: mirror = data & 3; break;
      case 13:
        irqEnable = data & 0x80;
        irqCounterEnable = data & 0x01;
        if(irqEnable == 0) cpu.irqLine(0);
        break;
      case 14: irqCounter = (irqCounter & 0xff00) | (data << 0); break;
      case 15: irqCounter = (irqCounter & 0x00ff) | (data << 8); break;
      }
    }

    if(addr == 0xc000) {
      apuPort = data & 0x0f;
    }

    if(addr == 0xe000) {
      switch(apuPort) {
      case  0: pulse[0].frequency = (pulse[0].frequency & 0xff00) | (data << 0); break;
      case  1: pulse[0].frequency = (pulse[0].frequency & 0x00ff) | (data << 8); break;
      case  2: pulse[1].frequency = (pulse[1].frequency & 0xff00) | (data << 0); break;
      case  3: pulse[1].frequency = (pulse[1].frequency & 0x00ff) | (data << 8); break;
      case  4: pulse[2].frequency = (pulse[2].frequency & 0xff00) | (data << 0); break;
      case  5: pulse[2].frequency = (pulse[2].frequency & 0x00ff) | (data << 8); break;
      case  7:
        pulse[0].disable = data & 0x01;
        pulse[1].disable = data & 0x02;
        pulse[2].disable = data & 0x04;
        break;
      case  8: pulse[0].volume = data & 0x0f; break;
      case  9: pulse[1].volume = data & 0x0f; break;
      case 10: pulse[2].volume = data & 0x0f; break;
      }
    }
  }

  auto addrCHR(uint addr) -> uint {
    uint8 bank = (addr >> 10) & 7;
    return (chrBank[bank] << 10) | (addr & 0x03ff);
  }

  auto addrCIRAM(uint addr) -> uint {
    switch(mirror) {
    case 0: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical
    case 1: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal
    case 2: return 0x0000 | (addr & 0x03ff);  //first
    case 3: return 0x0400 | (addr & 0x03ff);  //second
    }
  }

  auto readCHR(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(addrCIRAM(addr));
    return Board::readCHR(addrCHR(addr));
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(addrCIRAM(addr), data);
    return Board::writeCHR(addrCHR(addr), data);
  }

  auto power() -> void {
    for(int n : range(16)) {
      double volume = 1.0 / pow(2, 1.0 / 2 * (15 - n));
      dac[n] = volume * 8192.0;
    }

    mmuPort = 0;
    apuPort = 0;

    for(auto& n : prgBank) n = 0;
    for(auto& n : chrBank) n = 0;
    mirror = 0;
    irqEnable = 0;
    irqCounterEnable = 0;
    irqCounter = 0;

    pulse[0].power();
    pulse[1].power();
    pulse[2].power();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);

    s.integer(mmuPort);
    s.integer(apuPort);

    s.array(prgBank);
    s.array(chrBank);
    s.integer(mirror);
    s.integer(irqEnable);
    s.integer(irqCounterEnable);
    s.integer(irqCounter);

    pulse[0].serialize(s);
    pulse[1].serialize(s);
    pulse[2].serialize(s);
  }

  uint4 mmuPort;
  uint4 apuPort;

  uint8 prgBank[4];
  uint8 chrBank[8];
  uint2 mirror;
  bool irqEnable;
  bool irqCounterEnable;
  uint16 irqCounter;

  int16 dac[16];
};
