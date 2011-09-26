UOROM uorom;

uint8 UOROM::prg_read(uint16 addr) {
  if((addr & 0xc000) == 0x8000) {
    return prg_data((prg_bank << 14) | (addr & 0x3fff));
  }

  if((addr & 0xc000) == 0xc000) {
    return prg_data((0x0f << 14) | (addr & 0x3fff));
  }
}

void UOROM::prg_write(uint16 addr, uint8 data) {
  if(addr & 0x8000) prg_bank = data & 0x0f;
}

uint8 UOROM::chr_read(uint16 addr) {
  return chr_data(addr);
}

void UOROM::chr_write(uint16 addr, uint8 data) {
  if(cartridge.chr_ram == false) return;
  chr_data(addr) = data;
}

uint8 UOROM::ciram_read(uint13 addr) {
  if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  return ppu.ciram_read(addr);
}

void UOROM::ciram_write(uint13 addr, uint8 data) {
  if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  return ppu.ciram_write(addr, data);
}

void UOROM::power() {
  reset();
}

void UOROM::reset() {
  prg_bank = 0;
}

void UOROM::serialize(serializer &s) {
  s.integer(prg_bank);
}
