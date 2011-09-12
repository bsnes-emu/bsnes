uint8 None::prg_read(uint16 addr) {
  if(addr >= 0x8000 && addr <= 0xffff) {
    addr &= 0x7fff;
    if(addr >= cartridge.prg_size) addr &= cartridge.prg_size - 1;
    return cartridge.prg_data[addr];
  }
}

void None::prg_write(uint16 addr, uint8 data) {
}

uint8 None::chr_read(uint16 addr) {
  if(addr <= 0x1fff) {
    return cartridge.chr_data[addr & 0x1fff];
  }

  if(addr <= 0x3eff) {
    if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_read(addr & 0x07ff);
  }

  return ppu.cgram_read(addr & 0x001f);
}

void None::chr_write(uint16 addr, uint8 data) {
  if(addr <= 0x1fff) {
    if(cartridge.chr_ram == false) return;
    cartridge.chr_data[addr & 0x1fff] = data;
  }

  if(addr <= 0x3eff) {
    if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_write(addr & 0x07ff, data);
  }

  return ppu.cgram_write(addr & 0x001f, data);
}

void None::power() {
}

void None::reset() {
}
