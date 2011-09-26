CNROM cnrom;

uint8 CNROM::prg_read(unsigned addr) {
  if(addr & 0x8000) return Mapper::prg_read(addr & 0x7fff);
  return cpu.mdr();
}

void CNROM::prg_write(unsigned addr, uint8 data) {
  if(addr & 0x8000) chr_bank = data & 0x03;
}

uint8 CNROM::chr_read(unsigned addr) {
  if(addr & 0x2000) {
    if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_read(addr & 0x07ff);
  }

  addr = (chr_bank * 0x2000) + (addr & 0x1fff);
  return Mapper::chr_read(addr);
}

void CNROM::chr_write(unsigned addr, uint8 data) {
  if(addr & 0x2000) {
    if(cartridge.mirroring == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
    return ppu.ciram_write(addr & 0x07ff, data);
  }

  addr = (chr_bank * 0x2000) + (addr & 0x1fff);
  Mapper::chr_write(addr, data);
}

void CNROM::power() {
  reset();
}

void CNROM::reset() {
  chr_bank = 0;
}

void CNROM::serialize(serializer &s) {
  s.integer(chr_bank);
}
