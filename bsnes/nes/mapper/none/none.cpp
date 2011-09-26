None none;

uint8 None::prg_read(unsigned addr) {
  if(addr & 0x8000) return Mapper::prg_read(addr);
  return cpu.mdr();
}

void None::prg_write(unsigned addr, uint8 data) {
}

uint8 None::chr_read(unsigned addr) {
  if(addr & 0x2000) {
    if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_read(addr & 0x07ff);
  }
  return Mapper::chr_read(addr);
}

void None::chr_write(unsigned addr, uint8 data) {
  if(addr & 0x2000) {
    if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_write(addr & 0x07ff, data);
  }
  return Mapper::chr_write(addr, data);
}

void None::power() {
}

void None::reset() {
}

void None::serialize(serializer &s) {
}
