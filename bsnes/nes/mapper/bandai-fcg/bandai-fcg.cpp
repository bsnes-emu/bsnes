BandaiFCG bandaiFCG;

uint8 BandaiFCG::prg_read(uint16 addr) {
  clock();

  if(addr >= 0x8000 && addr <= 0xbfff) {
    unsigned rom_addr = (prg_bank << 14) | (addr & 0x3fff);
    return cartridge.prg_data[mirror(rom_addr, cartridge.prg_size)];
  }

  if(addr >= 0xc000 && addr <= 0xffff) {
    unsigned rom_addr = (0x0f << 14) | (addr & 0x3fff);
    return cartridge.prg_data[mirror(rom_addr, cartridge.prg_size)];
  }

  return cpu.mdr();
}

void BandaiFCG::prg_write(uint16 addr, uint8 data) {
  clock();

  if(addr >= 0x6000) {
    addr &= 0x0f;
    switch(addr) {
    case 0x0: case 0x1: case 0x2: case 0x3: case 0x4: case 0x5: case 0x6: case 0x7:
      chr_bank[addr] = data;
      break;
    case 0x8:
      prg_bank = data & 0x0f;
      break;
    case 0x9:
      mirror_select = data & 0x03;
      break;
    case 0xa:
      cpu.set_irq_line(0);
      irq_counter_enable = data & 0x01;
      irq_counter = irq_latch;
      break;
    case 0xb:
      irq_latch = (irq_latch & 0xff00) | (data << 0);
      break;
    case 0xc:
      irq_latch = (irq_latch & 0x00ff) | (data << 8);
      break;
    case 0xd:
    //TODO: serial EEPROM support
      break;
    }
  }
}

uint8 BandaiFCG::chr_read(uint16 addr) {
  unsigned rom_addr = (chr_bank[addr >> 10] << 10) | (addr & 0x03ff);
  return cartridge.chr_data[mirror(rom_addr, cartridge.chr_size)];
}

void BandaiFCG::chr_write(uint16 addr, uint8 data) {
  if(cartridge.chr_ram == false) return;
  unsigned rom_addr = (chr_bank[addr >> 10] << 10) | (addr & 0x03ff);
  cartridge.chr_data[mirror(rom_addr, cartridge.chr_size)] = data;
}

uint8 BandaiFCG::ciram_read(uint13 addr) {
  addr = ciram_addr(addr);
  return ppu.ciram_read(addr);
}

void BandaiFCG::ciram_write(uint13 addr, uint8 data) {
  addr = ciram_addr(addr);
  return ppu.ciram_write(addr, data);
}

void BandaiFCG::power() {
  reset();
}

void BandaiFCG::reset() {
  for(unsigned n = 0; n < 8; n++) chr_bank[n] = 0x00;
  prg_bank = 0x00;
  mirror_select = 0;
  irq_counter_enable = false;
  irq_counter = 0;
  irq_latch = 0;
}

//

unsigned BandaiFCG::ciram_addr(unsigned addr) const {
  switch(mirror_select & 0x03) {
  case 0: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical mirroring
  case 1: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal mirroring
  case 2: return 0x0000 | (addr & 0x03ff);                  //one-screen mirroring (first)
  case 3: return 0x0400 | (addr & 0x03ff);                  //one-screen mirroring (second)
  }
  throw;
}

void BandaiFCG::clock() {
  if(irq_counter_enable) {
    if(--irq_counter == 0xffff) {
      cpu.set_irq_line(1);
      irq_counter_enable = false;
    }
  }
}

//

void BandaiFCG::serialize(serializer &s) {
  s.array(chr_bank);
  s.integer(prg_bank);
  s.integer(mirror_select);
  s.integer(irq_counter_enable);
  s.integer(irq_counter);
  s.integer(irq_latch);
}
