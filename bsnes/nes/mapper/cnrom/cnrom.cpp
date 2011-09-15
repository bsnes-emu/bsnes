CNROM cnrom;

uint8 CNROM::prg_read(uint16 addr) {
  if(addr & 0x8000) {
    return cartridge.prg_data[mirror(addr & 0x7fff, cartridge.prg_size)];
  }

  return cpu.mdr();
}

void CNROM::prg_write(uint16 addr, uint8 data) {
  if(addr & 0x8000) chr_bank = data & 0x03;
}

uint8 CNROM::chr_read(uint16 addr) {
  unsigned chr_addr = (chr_bank * 0x2000) + (addr & 0x1fff);
  return cartridge.chr_data[mirror(chr_addr, cartridge.chr_size)];
}

void CNROM::chr_write(uint16 addr, uint8 data) {
  if(cartridge.chr_ram == false) return;
  unsigned chr_addr = (chr_bank * 0x2000) + (addr & 0x1fff);
  cartridge.chr_data[mirror(chr_addr, cartridge.chr_size)] = data;
}

uint8 CNROM::ciram_read(uint13 addr) {
  if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  return ppu.ciram_read(addr & 0x07ff);
}

void CNROM::ciram_write(uint13 addr, uint8 data) {
  if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  return ppu.ciram_write(addr & 0x07ff, data);
}

void CNROM::power() {
  reset();
}

void CNROM::reset() {
  chr_bank = 0;
}
