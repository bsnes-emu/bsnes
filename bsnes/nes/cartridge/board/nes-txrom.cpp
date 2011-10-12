struct NES_TxROM : Board {

enum class Revision : unsigned {
  TBROM,
  TEROM,
  TFROM,
  TGROM,
  TKROM,
  TKSROM,
  TLROM,
  TL1ROM,
  TL2ROM,
  TLSROM,
  TNROM,
  TQROM,
  TR1ROM,
  TSROM,
  TVROM,
} revision;

MMC3 mmc3;

void main() {
  mmc3.main();
}

uint8 prg_read(unsigned addr) {
  if((addr & 0xe000) == 0x6000) return mmc3.ram_read(addr);
  if(addr & 0x8000) return prgrom.read(mmc3.prg_addr(addr));
  return cpu.mdr();
}

void prg_write(unsigned addr, uint8 data) {
  if((addr & 0xe000) == 0x6000) return mmc3.ram_write(addr, data);

  switch(addr & 0xe001) {
  case 0x8000:
    mmc3.chr_mode = data & 0x80;
    mmc3.prg_mode = data & 0x40;
    mmc3.bank_select = data & 0x07;
    break;

  case 0x8001:
    switch(mmc3.bank_select) {
    case 0: mmc3.chr_bank[0] = data & ~1; break;
    case 1: mmc3.chr_bank[1] = data & ~1; break;
    case 2: mmc3.chr_bank[2] = data; break;
    case 3: mmc3.chr_bank[3] = data; break;
    case 4: mmc3.chr_bank[4] = data; break;
    case 5: mmc3.chr_bank[5] = data; break;
    case 6: mmc3.prg_bank[0] = data & 0x3f; break;
    case 7: mmc3.prg_bank[1] = data & 0x3f; break;
    }
    break;

  case 0xa000:
    mmc3.mirror = data & 0x01;
    break;

  case 0xa001:
    mmc3.ram_enable = data & 0x80;
    mmc3.ram_write_protect = data & 0x40;
    break;

  case 0xc000:
    mmc3.irq_latch = data;
    break;

  case 0xc001:
    mmc3.irq_counter = 0;
    break;

  case 0xe000:
    mmc3.irq_enable = false;
    mmc3.irq_line = 0;
    break;

  case 0xe001:
    mmc3.irq_enable = true;
    break;
  }
}

uint8 chr_read(unsigned addr) {
  mmc3.irq_test(addr);
  if(addr & 0x2000) return ppu.ciram_read(mmc3.ciram_addr(addr));
  return Board::chr_read(mmc3.chr_addr(addr));
}

void chr_write(unsigned addr, uint8 data) {
  mmc3.irq_test(addr);
  if(addr & 0x2000) return ppu.ciram_write(mmc3.ciram_addr(addr), data);
  return Board::chr_write(mmc3.chr_addr(addr), data);
}

void power() {
  mmc3.power();
}

void reset() {
  mmc3.reset();
}

void serialize(serializer &s) {
  Board::serialize(s);
  mmc3.serialize(s);
}

NES_TxROM(BML::Node &board, const uint8_t *data, unsigned size) : Board(board, data, size), mmc3(*this) {
  revision = Revision::TLROM;
}

};
