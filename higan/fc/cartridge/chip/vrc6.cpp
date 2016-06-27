struct VRC6 : Chip {
  VRC6(Board& board) : Chip(board) {
  }

  struct Pulse {
    auto clock() -> void {
      if(--divider == 0) {
        divider = frequency + 1;
        cycle++;
        output = (mode == 1 || cycle > duty) ? volume : (uint4)0;
      }

      if(enable == false) output = 0;
    }

    auto serialize(serializer& s) -> void {
      s.integer(mode);
      s.integer(duty);
      s.integer(volume);
      s.integer(enable);
      s.integer(frequency);

      s.integer(divider);
      s.integer(cycle);
      s.integer(output);
    }

    bool mode;
    uint3 duty;
    uint4 volume;
    bool enable;
    uint12 frequency;

    uint12 divider;
    uint4 cycle;
    uint4 output;
  } pulse1, pulse2;

  struct Sawtooth {
    auto clock() -> void {
      if(--divider == 0) {
        divider = frequency + 1;
        if(++phase == 0) {
          accumulator += rate;
          if(++stage == 7) {
            stage = 0;
            accumulator = 0;
          }
        }
      }

      output = accumulator >> 3;
      if(enable == false) output = 0;
    }

    auto serialize(serializer& s) -> void {
      s.integer(rate);
      s.integer(enable);
      s.integer(frequency);

      s.integer(divider);
      s.integer(phase);
      s.integer(stage);
      s.integer(accumulator);
      s.integer(output);
    }

    uint6 rate;
    bool enable;
    uint12 frequency;

    uint12 divider;
    uint1 phase;
    uint3 stage;
    uint8 accumulator;
    uint5 output;
  } sawtooth;

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

    pulse1.clock();
    pulse2.clock();
    sawtooth.clock();
    int output = (pulse1.output + pulse2.output + sawtooth.output) << 7;
    apu.setSample(-output);

    tick();
  }

  auto addrPRG(uint addr) const -> uint {
    if((addr & 0xc000) == 0x8000) return (prgBank[0] << 14) | (addr & 0x3fff);
    if((addr & 0xe000) == 0xc000) return (prgBank[1] << 13) | (addr & 0x1fff);
    if((addr & 0xe000) == 0xe000) return (      0xff << 13) | (addr & 0x1fff);
  }

  auto addrCHR(uint addr) const -> uint {
    uint bank = chrBank[(addr >> 10) & 7];
    return (bank << 10) | (addr & 0x03ff);
  }

  auto addrCIRAM(uint addr) const -> uint {
    switch(mirror) {
    case 0: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical mirroring
    case 1: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal mirroring
    case 2: return 0x0000 | (addr & 0x03ff);                  //one-screen mirroring (first)
    case 3: return 0x0400 | (addr & 0x03ff);                  //one-screen mirroring (second)
    }
  }

  auto readRAM(uint addr) -> uint8 {
    return board.prgram.data[addr & 0x1fff];
  }

  auto writeRAM(uint addr, uint8 data) -> void {
    board.prgram.data[addr & 0x1fff] = data;
  }

  auto writeIO(uint addr, uint8 data) -> void {
    switch(addr) {
    case 0x8000: case 0x8001: case 0x8002: case 0x8003:
      prgBank[0] = data;
      break;

    case 0x9000:
      pulse1.mode = data & 0x80;
      pulse1.duty = (data & 0x70) >> 4;
      pulse1.volume = data & 0x0f;
      break;

    case 0x9001:
      pulse1.frequency = (pulse1.frequency & 0x0f00) | ((data & 0xff) << 0);
      break;

    case 0x9002:
      pulse1.frequency = (pulse1.frequency & 0x00ff) | ((data & 0x0f) << 8);
      pulse1.enable = data & 0x80;
      break;

    case 0xa000:
      pulse2.mode = data & 0x80;
      pulse2.duty = (data & 0x70) >> 4;
      pulse2.volume = data & 0x0f;
      break;

    case 0xa001:
      pulse2.frequency = (pulse2.frequency & 0x0f00) | ((data & 0xff) << 0);
      break;

    case 0xa002:
      pulse2.frequency = (pulse2.frequency & 0x00ff) | ((data & 0x0f) << 8);
      pulse2.enable = data & 0x80;
      break;

    case 0xb000:
      sawtooth.rate = data & 0x3f;
      break;

    case 0xb001:
      sawtooth.frequency = (sawtooth.frequency & 0x0f00) | ((data & 0xff) << 0);
      break;

    case 0xb002:
      sawtooth.frequency = (sawtooth.frequency & 0x00ff) | ((data & 0x0f) << 8);
      sawtooth.enable = data & 0x80;
      break;

    case 0xb003:
      mirror = (data >> 2) & 3;
      break;

    case 0xc000: case 0xc001: case 0xc002: case 0xc003:
      prgBank[1] = data;
      break;

    case 0xd000: case 0xd001: case 0xd002: case 0xd003:
      chrBank[0 + (addr & 3)] = data;
      break;

    case 0xe000: case 0xe001: case 0xe002: case 0xe003:
      chrBank[4 + (addr & 3)] = data;
      break;

    case 0xf000:
      irqLatch = data;
      break;

    case 0xf001:
      irqMode = data & 0x04;
      irqEnable = data & 0x02;
      irqAcknowledge = data & 0x01;
      if(irqEnable) {
        irqCounter = irqLatch;
        irqScalar = 341;
      }
      irqLine = 0;
      break;

    case 0xf002:
      irqEnable = irqAcknowledge;
      irqLine = 0;
      break;
    }
  }

  auto power() -> void {
  }

  auto reset() -> void {
    prgBank[0] = 0;
    prgBank[1] = 0;
    chrBank[0] = 0;
    chrBank[1] = 0;
    chrBank[2] = 0;
    chrBank[3] = 0;
    chrBank[4] = 0;
    chrBank[5] = 0;
    chrBank[6] = 0;
    chrBank[7] = 0;
    mirror = 0;
    irqLatch = 0;
    irqMode = 0;
    irqEnable = 0;
    irqAcknowledge = 0;

    irqCounter = 0;
    irqScalar = 0;
    irqLine = 0;

    pulse1.mode = 0;
    pulse1.duty = 0;
    pulse1.volume = 0;
    pulse1.enable = 0;
    pulse1.frequency = 0;

    pulse1.divider = 1;
    pulse1.cycle = 0;
    pulse1.output = 0;

    pulse2.mode = 0;
    pulse2.duty = 0;
    pulse2.volume = 0;
    pulse2.enable = 0;
    pulse2.frequency = 0;

    pulse2.divider = 1;
    pulse2.cycle = 0;
    pulse2.output = 0;

    sawtooth.rate = 0;
    sawtooth.enable = 0;
    sawtooth.frequency = 0;

    sawtooth.divider = 1;
    sawtooth.phase = 0;
    sawtooth.stage = 0;
    sawtooth.accumulator = 0;
    sawtooth.output = 0;
  }

  auto serialize(serializer& s) -> void {
    pulse1.serialize(s);
    pulse2.serialize(s);
    sawtooth.serialize(s);

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

  uint8 prgBank[2];
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
