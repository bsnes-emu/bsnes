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

    auto reset() -> void {
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
    if(irq_counter_enable) {
      if(--irq_counter == 0xffff) {
        cpu.irqLine(irq_enable);
      }
    }

    pulse[0].clock();
    pulse[1].clock();
    pulse[2].clock();
    int16 output = dac[pulse[0].output] + dac[pulse[1].output] + dac[pulse[2].output];
    apu.set_sample(-output);

    tick();
  }

  auto prg_read(uint addr) -> uint8 {
    if(addr < 0x6000) return cpu.mdr();

    uint8 bank = 0x3f;  //((addr & 0xe000) == 0xe000
    if((addr & 0xe000) == 0x6000) bank = prg_bank[0];
    if((addr & 0xe000) == 0x8000) bank = prg_bank[1];
    if((addr & 0xe000) == 0xa000) bank = prg_bank[2];
    if((addr & 0xe000) == 0xc000) bank = prg_bank[3];

    bool ram_enable = bank & 0x80;
    bool ram_select = bank & 0x40;
    bank &= 0x3f;

    if(ram_select) {
      if(ram_enable == false) return cpu.mdr();
      return prgram.data[addr & 0x1fff];
    }

    addr = (bank << 13) | (addr & 0x1fff);
    return prgrom.read(addr);
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if((addr & 0xe000) == 0x6000) {
      prgram.data[addr & 0x1fff] = data;
    }

    if(addr == 0x8000) {
      mmu_port = data & 0x0f;
    }

    if(addr == 0xa000) {
      switch(mmu_port) {
      case  0: chr_bank[0] = data; break;
      case  1: chr_bank[1] = data; break;
      case  2: chr_bank[2] = data; break;
      case  3: chr_bank[3] = data; break;
      case  4: chr_bank[4] = data; break;
      case  5: chr_bank[5] = data; break;
      case  6: chr_bank[6] = data; break;
      case  7: chr_bank[7] = data; break;
      case  8: prg_bank[0] = data; break;
      case  9: prg_bank[1] = data; break;
      case 10: prg_bank[2] = data; break;
      case 11: prg_bank[3] = data; break;
      case 12: mirror = data & 3; break;
      case 13:
        irq_enable = data & 0x80;
        irq_counter_enable = data & 0x01;
        if(irq_enable == 0) cpu.irqLine(0);
        break;
      case 14: irq_counter = (irq_counter & 0xff00) | (data << 0); break;
      case 15: irq_counter = (irq_counter & 0x00ff) | (data << 8); break;
      }
    }

    if(addr == 0xc000) {
      apu_port = data & 0x0f;
    }

    if(addr == 0xe000) {
      switch(apu_port) {
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

  auto chr_addr(uint addr) -> uint {
    uint8 bank = (addr >> 10) & 7;
    return (chr_bank[bank] << 10) | (addr & 0x03ff);
  }

  auto ciram_addr(uint addr) -> uint {
    switch(mirror) {
    case 0: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical
    case 1: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal
    case 2: return 0x0000 | (addr & 0x03ff);  //first
    case 3: return 0x0400 | (addr & 0x03ff);  //second
    }
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(ciram_addr(addr));
    return Board::chr_read(chr_addr(addr));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(ciram_addr(addr), data);
    return Board::chr_write(chr_addr(addr), data);
  }

  auto power() -> void {
    for(signed n : range(16)) {
      double volume = 1.0 / pow(2, 1.0 / 2 * (15 - n));
      dac[n] = volume * 8192.0;
    }
  }

  auto reset() -> void {
    mmu_port = 0;
    apu_port = 0;

    for(auto& n : prg_bank) n = 0;
    for(auto& n : chr_bank) n = 0;
    mirror = 0;
    irq_enable = 0;
    irq_counter_enable = 0;
    irq_counter = 0;

    pulse[0].reset();
    pulse[1].reset();
    pulse[2].reset();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);

    s.integer(mmu_port);
    s.integer(apu_port);

    s.array(prg_bank);
    s.array(chr_bank);
    s.integer(mirror);
    s.integer(irq_enable);
    s.integer(irq_counter_enable);
    s.integer(irq_counter);

    pulse[0].serialize(s);
    pulse[1].serialize(s);
    pulse[2].serialize(s);
  }

  uint4 mmu_port;
  uint4 apu_port;

  uint8 prg_bank[4];
  uint8 chr_bank[8];
  uint2 mirror;
  bool irq_enable;
  bool irq_counter_enable;
  uint16 irq_counter;

  int16 dac[16];
};
