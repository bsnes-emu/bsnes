struct KonamiVRC2 : Board {

VRC2 vrc2;
bool latch;

uint8 prg_read(unsigned addr) {
  if(addr == 0x6000) return latch;
  if(addr & 0x8000) return prgrom.read(vrc2.prg_addr(addr));
  return cpu.mdr();
}

void prg_write(unsigned addr, uint8 data) {
  if(addr == 0x6000) latch = data & 0x01;
  if(addr & 0x8000) return vrc2.reg_write(addr, data);
}

uint8 chr_read(unsigned addr) {
  if(addr & 0x2000) return ppu.ciram_read(vrc2.ciram_addr(addr));
  return Board::chr_read(vrc2.chr_addr(addr));
}

void chr_write(unsigned addr, uint8 data) {
  if(addr & 0x2000) return ppu.ciram_write(vrc2.ciram_addr(addr), data);
  return Board::chr_write(vrc2.chr_addr(addr), data);
}

void power() {
  vrc2.power();
}

void reset() {
  vrc2.reset();
  latch = 0;
}

void serialize(serializer &s) {
  Board::serialize(s);
  vrc2.serialize(s);
  s.integer(latch);
}

KonamiVRC2(BML::Node &board, const uint8_t *data, unsigned size) : Board(board, data, size), vrc2(*this) {
}

};
