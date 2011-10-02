struct NES_SxROM : Board {

enum class Revision : unsigned {
  SAROM,
  SBROM,
  SCROM,
  SC1ROM,
  SEROM,
  SFROM,
  SGROM,
  SHROM,
  SH1ROM,
  SIROM,
  SJROM,
  SKROM,
  SLROM,
  SL1ROM,
  SL2ROM,
  SL3ROM,
  SLRROM,
  SMROM,
  SNROM,
  SOROM,
  SUROM,
  SXROM,
} revision;

MMC1 mmc1;

unsigned shiftaddr;
unsigned shiftdata;

uint8 prg_read(unsigned addr) {
  if((addr & 0xe000) == 0x6000) return mmc1.ram_read(addr);
  if(addr & 0x8000) return Board::prg_read(mmc1.prg_addr(addr));
  return cpu.mdr();
}

void prg_write(unsigned addr, uint8 data) {
  if((addr & 0xe000) == 0x6000) return mmc1.ram_write(addr, data);

  if(addr & 0x8000) {
    if(data & 0x80) {
      shiftaddr = 0;
      mmc1.prg_size = 1;
      mmc1.prg_mode = 1;
    } else {
      shiftdata = ((data & 1) << 4) | (shiftdata >> 1);
      if(++shiftaddr == 5) {
        shiftaddr = 0;
        reg_write((addr >> 13) & 3, shiftdata);
      }
    }
  }
}

uint8 chr_read(unsigned addr) {
  if(addr & 0x2000) return ppu.ciram_read(mmc1.ciram_addr(addr));
  return Board::chr_read(mmc1.chr_addr(addr));
}

void chr_write(unsigned addr, uint8 data) {
  if(addr & 0x2000) return ppu.ciram_write(mmc1.ciram_addr(addr), data);
  return Board::chr_write(mmc1.chr_addr(addr), data);
}

void reg_write(unsigned addr, uint8 data) {
  switch(addr) {
  case 0:
    mmc1.chr_mode = (data & 0x10);
    mmc1.prg_size = (data & 0x08);
    mmc1.prg_mode = (data & 0x04);
    mmc1.mirror = (data & 0x03);
    break;

  case 1:
    mmc1.chr_bank[0] = (data & 0x1f);
    switch(revision) {
    case Revision::SNROM:
      mmc1.ram_disable[1] = (data & 0x10);
      break;
    case Revision::SOROM:
      mmc1.ram_bank = (data & 0x08) >> 3;
      break;
    case Revision::SUROM:
      mmc1.prg_page = (data & 0x10);
      break;
    case Revision::SXROM:
      mmc1.prg_page = (data & 0x10);
      mmc1.ram_bank = (data & 0x0c) >> 2;
      break;
    }
    break;

  case 2:
    mmc1.chr_bank[1] = (data & 0x1f);
    switch(revision) {
    case Revision::SNROM:
      mmc1.ram_disable[1] = (data & 0x10);
      break;
    case Revision::SOROM:
      mmc1.ram_bank = (data & 0x08) >> 3;
      break;
    case Revision::SUROM:
      mmc1.prg_page = (data & 0x10);
      break;
    case Revision::SXROM:
      mmc1.prg_page = (data & 0x10);
      mmc1.ram_bank = (data & 0x0c) >> 2;
      break;
    }
    break;

  case 3:
    mmc1.ram_disable[0] = (data & 0x10);
    mmc1.prg_bank = (data & 0x0f);
    break;
  }
}

Memory memory() {
  return prgram;
}

void power() {
  mmc1.power();
}

void reset() {
  mmc1.reset();
}

void serialize(serializer &s) {
  Board::serialize(s);
  mmc1.serialize(s);

  s.integer(shiftaddr);
  s.integer(shiftdata);
}

NES_SxROM(BML::Node &board, const uint8_t *data, unsigned size) : Board(board, data, size), mmc1(*this) {
  revision = Revision::SXROM;

  shiftaddr = 0;
  shiftdata = 0;
}

};
