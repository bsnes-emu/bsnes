UOROM uorom;

uint8 UOROM::prg_read(unsigned addr) {
  if((addr & 0xc000) == 0x8000) {
    return Mapper::prg_read((prg_bank << 14) | (addr & 0x3fff));
  }

  if((addr & 0xc000) == 0xc000) {
    return Mapper::prg_read((0x0f << 14) | (addr & 0x3fff));
  }
}

void UOROM::prg_write(unsigned addr, uint8 data) {
  if(addr & 0x8000) prg_bank = data & 0x0f;
}

uint8 UOROM::chr_read(unsigned addr) {
  if(addr & 0x2000) {
    if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_read(addr);
  }

  return Mapper::chr_read(addr);
}

void UOROM::chr_write(unsigned addr, uint8 data) {
  if(addr & 0x2000) {
    if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_write(addr, data);
  }

  return Mapper::chr_write(addr, data);
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
