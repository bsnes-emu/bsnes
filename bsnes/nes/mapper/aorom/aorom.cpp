AOROM aorom;

uint8 AOROM::prg_read(unsigned addr) {
  if(addr & 0x8000) {
    addr = (prg_bank << 15) | (addr & 0x7fff);
    return Mapper::prg_read(addr);
  }
  return cpu.mdr();
}

void AOROM::prg_write(unsigned addr, uint8 data) {
  if(addr & 0x8000) {
    prg_bank = data & 0x0f;
    mirror_select = data & 0x10;
  }
}

uint8 AOROM::chr_read(unsigned addr) {
  if(addr & 0x2000) return ppu.ciram_read((mirror_select << 10) | (addr & 0x03ff));
  return Mapper::chr_read(addr);
}

void AOROM::chr_write(unsigned addr, uint8 data) {
  if(addr & 0x2000) return ppu.ciram_write((mirror_select << 10) | (addr & 0x03ff), data);
  return Mapper::chr_write(addr, data);
}

void AOROM::power() {
  reset();
}

void AOROM::reset() {
  prg_bank = 0x0f;
  mirror_select = 0;
}

void AOROM::serialize(serializer &s) {
  s.integer(prg_bank);
  s.integer(mirror_select);
}
