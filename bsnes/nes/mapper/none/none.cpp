None none;

uint8 None::prg_read(uint16 addr) {
  if(addr >= 0x8000 && addr <= 0xffff) {
    addr &= 0x7fff;
    if(addr >= cartridge.prg_size) addr &= cartridge.prg_size - 1;
    return cartridge.prg_data[addr];
  }

  return cpu.mdr();
}

void None::prg_write(uint16 addr, uint8 data) {
}

uint8 None::chr_read(uint16 addr) {
  return cartridge.chr_data[addr & 0x1fff];
}

void None::chr_write(uint16 addr, uint8 data) {
  if(cartridge.chr_ram == false) return;
  cartridge.chr_data[addr & 0x1fff] = data;
}

uint8 None::ciram_read(uint13 addr) {
  if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  return ppu.ciram_read(addr & 0x07ff);
}

void None::ciram_write(uint13 addr, uint8 data) {
  if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  return ppu.ciram_write(addr & 0x07ff, data);
}

void None::power() {
}

void None::reset() {
}

void None::serialize(serializer &s) {
}
