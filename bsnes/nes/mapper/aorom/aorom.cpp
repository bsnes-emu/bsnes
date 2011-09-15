AOROM aorom;

uint8 AOROM::prg_read(uint16 addr) {
  if(addr & 0x8000) {
    unsigned rom_addr = (prg_bank << 15) | (addr & 0x7fff);
    return cartridge.prg_data[mirror(rom_addr, cartridge.prg_size)];
  }

  return cpu.mdr();
}

void AOROM::prg_write(uint16 addr, uint8 data) {
  if(addr & 0x8000) {
    prg_bank = data & 0x0f;
    mirror_select = data & 0x10;
  }
}

uint8 AOROM::chr_read(uint16 addr) {
  return cartridge.chr_data[mirror(addr, cartridge.chr_size)];
}

void AOROM::chr_write(uint16 addr, uint8 data) {
  if(cartridge.chr_ram == false) return;
  cartridge.chr_data[mirror(addr, cartridge.chr_size)] = data;
}

uint8 AOROM::ciram_read(uint13 addr) {
  return ppu.ciram_read((mirror_select << 10) | (addr & 0x03ff));
}

void AOROM::ciram_write(uint13 addr, uint8 data) {
  return ppu.ciram_write((mirror_select << 10) | (addr & 0x03ff), data);
}

void AOROM::power() {
  reset();
}

void AOROM::reset() {
  prg_bank = 0x0f;
  mirror_select = 0;
}
