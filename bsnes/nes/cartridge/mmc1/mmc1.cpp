unsigned MMC1::ciram_addr(unsigned addr) const {
  switch(r[0] & 0x03) {
  case 0: return 0x0000 | (addr & 0x03ff);                  //one-screen mirroring (first)
  case 1: return 0x0400 | (addr & 0x03ff);                  //one-screen mirroring (second)
  case 2: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical mirroring
  case 3: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal mirroring
  }
}

bool MMC1::prg_mode() const {
  return r[0] & 0x04;
}

unsigned MMC1::prg_addr() const {
  if(r[0] & 0x08) return prg_bank() * 0x4000;
  return (prg_bank() & ~1) * 0x8000;
}

bool MMC1::chr_mode() const {
  return r[0] & 0x10;
}

unsigned MMC1::chr_banklo() const {
  return r[1];
}

unsigned MMC1::chr_bankhi() const {
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
    if(prg_mode() == 0) {
      if(addr >= 0x8000 && addr <= 0xbfff) {
        addr = (addr & 0x3fff);
        return prg_data(addr);
      }
      if(addr >= 0xc000 && addr <= 0xffff) {
        addr = prg_addr() | (addr & 0x3fff);
        return prg_data(addr);
      }
    }

    if(prg_mode() == 1) {
      if(addr >= 0x8000 && addr <= 0xbfff) {
        addr = prg_addr() | (addr & 0x3fff);
        return prg_data(addr);
      }
      if(addr >= 0xc000 && addr <= 0xffff) {
        addr = 15 * 0x4000 | (addr & 0x3fff);
        return prg_data(addr);
      }
    }
  }
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

  if(addr <= 0x0fff) {
    if(chr_mode() == 0) addr = chr_banklo() * 0x2000 + (addr & 0x1fff);
    if(chr_mode() == 1) addr = chr_banklo() * 0x1000 + (addr & 0x0fff);
    return chr_data(addr);
  }

  if(addr <= 0x1fff) {
    if(chr_mode() == 0) addr = chr_banklo() * 0x2000 + (addr & 0x1fff);
    if(chr_mode() == 1) addr = chr_bankhi() * 0x1000 + (addr & 0x0fff);
    return chr_data(addr);
  }

  if(addr <= 0x3eff) {
    addr = ciram_addr(addr);
    return ppu.ciram_read(addr);
  }

  return ppu.cgram_read(addr);
}

void MMC1::chr_write(uint16 readaddr, uint8 data) {
  unsigned addr = readaddr;

  if(addr <= 0x0fff) {
    if(cartridge.chr_ram == false) return;
    if(chr_mode() == 0) addr = chr_banklo() * 0x2000 + (addr & 0x1fff);
    if(chr_mode() == 1) addr = chr_banklo() * 0x1000 + (addr & 0x0fff);
    chr_data(addr) = data;
    return;
  }

  if(addr <= 0x1fff) {
    if(cartridge.chr_ram == false) return;
    if(chr_mode() == 0) addr = chr_banklo() * 0x2000 + (addr & 0x1fff);
    if(chr_mode() == 1) addr = chr_bankhi() * 0x1000 + (addr & 0x0fff);
    chr_data(addr) = data;
    return;
  }

  if(addr <= 0x3eff) {
    addr = ciram_addr(addr);
    return ppu.ciram_write(addr, data);
  }

  return ppu.cgram_write(addr, data);
}

//

void MMC1::power() {
  reset();
}

void MMC1::reset() {
  shiftaddr = 0;
  shiftdata = 0;

  r[0] = 0x0c;
  r[1] = 0x00;
  r[2] = 0x01;
  r[3] = 0x0f;
}
