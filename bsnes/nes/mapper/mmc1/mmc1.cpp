MMC1 mmc1;

unsigned MMC1::ciram_addr(unsigned addr) const {
  switch(r[0] & 0x03) {
  case 0: return 0x0000 | (addr & 0x03ff);                  //one-screen mirroring (first)
  case 1: return 0x0400 | (addr & 0x03ff);                  //one-screen mirroring (second)
  case 2: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical mirroring
  case 3: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal mirroring
  }
}

//static block: 0 = 8000-ffff, 1 = c000-ffff
bool MMC1::prg_mode() const {
  return r[0] & 0x04;
}

//block size: 0 = 32K, 1 = 16K
bool MMC1::prg_size() const {
  return r[0] & 0x08;
}

unsigned MMC1::prg_addr(bool region) const {
  unsigned addr, bank;

  //region(0) = $8000-bfff; region(1) = $c000-ffff
  if(prg_size()) {  //16K mode
    bank = (region == 0 ? 0x0 : 0xf);  //8000-bfff defaults to first bank; c000-ffff defaults to last bank
    if(region != prg_mode()) {  //if this is the active dynamic region ...
      bank = prg_bank();
    }
  } else {  //32K mode
    bank = (prg_bank() & ~1) + region;
  }
  addr = bank << 14;  //<<14 = 16K

  //256K page selection (for 512K PRG ROMs)
  if(prg_ex_select == 0) {
    addr |= (chr_banklo() >> 4) << 18;  //<<18 = 256K
  } else {
    addr |= (chr_bankhi() >> 4) << 18;
  }

  return addr;
}

//0 = 8K, 1 = 4K
bool MMC1::chr_mode() const {
  return r[0] & 0x10;
}

unsigned MMC1::chr_banklo() const {
  if(chr_mode() == 0) return (r[1] & ~1) | 0;
  return r[1];
}

unsigned MMC1::chr_bankhi() const {
  if(chr_mode() == 0) return (r[1] & ~1) | 1;
  return r[2];
}

unsigned MMC1::prg_bank() const {
  return r[3] & 0x0f;
}

bool MMC1::prg_ram_disable() const {
  return r[3] & 0x10;
}

//

uint8& MMC1::prg_data(unsigned addr) {
  return cartridge.prg_data[mirror(addr, cartridge.prg_size)];
}

uint8& MMC1::chr_data(unsigned addr) {
  return cartridge.chr_data[mirror(addr, cartridge.chr_size)];
}

//

uint8 MMC1::prg_read(uint16 readaddr) {
  unsigned addr = readaddr;

  if(addr >= 0x6000 && addr <= 0x7fff) {
    if(prg_ram_disable() == false) return prg_ram[addr & 0x1fff];
    return 0x00;
  }

  if(addr >= 0x8000 && addr <= 0xffff) {
    addr = prg_addr(addr & 0x4000) | (addr & 0x3fff);
    return prg_data(addr);
  }

  return cpu.mdr();
}

void MMC1::prg_write(uint16 addr, uint8 data) {
  if(addr >= 0x6000 && addr <= 0x7fff) {
    if(prg_ram_disable() == false) prg_ram[addr & 0x1fff] = data;
    return;
  }

  if(addr >= 0x8000 && addr <= 0xffff) {
    if(data & 0x80) {
      shiftaddr = 0;
      r[0] |= 0x0c;
    } else {
      shiftdata >>= 1;
      shiftdata |= (data & 1) << 4;
      if(++shiftaddr == 5) {
        shiftaddr = 0;
        r[(addr >> 13) & 3] = shiftdata;
      }
    }
    return;
  }
}

//

uint8 MMC1::chr_read(uint16 readaddr) {
  unsigned addr = readaddr;
  prg_ex_select = addr & 0x1000;

  if(addr <= 0x0fff) {
    addr = chr_banklo() * 0x1000 + (addr & 0x0fff);
    return chr_data(addr);
  }

  if(addr <= 0x1fff) {
    addr = chr_bankhi() * 0x1000 + (addr & 0x0fff);
    return chr_data(addr);
  }

  throw;
}

void MMC1::chr_write(uint16 readaddr, uint8 data) {
  unsigned addr = readaddr;
  prg_ex_select = addr & 0x1000;

  if(addr <= 0x0fff) {
    if(cartridge.chr_ram == false) return;
    addr = chr_banklo() * 0x1000 + (addr & 0x0fff);
    chr_data(addr) = data;
    return;
  }

  if(addr <= 0x1fff) {
    if(cartridge.chr_ram == false) return;
    addr = chr_bankhi() * 0x1000 + (addr & 0x0fff);
    chr_data(addr) = data;
    return;
  }

  throw;
}

//

uint8 MMC1::ciram_read(uint13 addr) {
  addr = ciram_addr(addr);
  return ppu.ciram_read(addr);
}

void MMC1::ciram_write(uint13 addr, uint8 data) {
  addr = ciram_addr(addr);
  return ppu.ciram_write(addr, data);
}

//

unsigned MMC1::ram_size() {
  return 8192u;
}

uint8* MMC1::ram_data() {
  return prg_ram;
}

//

void MMC1::power() {
  reset();
}

void MMC1::reset() {
  prg_ex_select = 0;
  shiftaddr = 0;
  shiftdata = 0;

  r[0] = 0x0c;
  r[1] = 0x00;
  r[2] = 0x01;
  r[3] = 0x00;
}

//

void MMC1::serialize(serializer &s) {
  s.array(prg_ram);
  s.array(r);
  s.integer(prg_ex_select);
  s.integer(shiftaddr);
  s.integer(shiftdata);
}
