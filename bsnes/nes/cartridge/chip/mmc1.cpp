struct MMC1 : Chip {

enum class Revision : unsigned {
  MMC1,
  MMC1A,
  MMC1B1,
  MMC1B2,
  MMC1B3,
  MMC1C,
} revision;

bool chr_mode;
bool prg_size;  //0 = 32K, 1 = 16K
bool prg_mode;
uint2 mirror;  //0 = first, 1 = second, 2 = vertical, 3 = horizontal
uint5 chr_bank[2];
uint4 prg_bank;
bool prg_page;
uint2 ram_bank;
bool ram_disable[2];

unsigned prg_addr(unsigned addr) const {
  bool region = addr & 0x4000;
  unsigned bank = (prg_bank & ~1) + region;

  if(prg_size) {
    bank = (region == 0 ? 0x0 : 0xf);
    if(region != prg_mode) bank = prg_bank;
  }

  return (prg_page << 18) | (bank << 14) | (addr & 0x3fff);
}

unsigned chr_addr(unsigned addr) const {
  bool region = addr & 0x1000;
  unsigned bank = chr_bank[region];
  if(chr_mode == 0) bank = (chr_bank[0] & ~1) | region;
  return (bank << 12) | (addr & 0x0fff);
}

unsigned ciram_addr(unsigned addr) const {
  switch(mirror) {
  case 0: return 0x0000 | (addr & 0x03ff);
  case 1: return 0x0400 | (addr & 0x03ff);
  case 2: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);
  case 3: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);
  }
}

uint8 ram_read(unsigned addr) {
  addr = (ram_bank * 0x2000) | (addr & 0x1fff);
  if(ram_disable[0] == false && ram_disable[1] == false) return board.prgram.data[addr];
  return 0x00;
}

void ram_write(unsigned addr, uint8 data) {
  addr = (ram_bank * 0x2000) | (addr & 0x1fff);
  if(ram_disable[0] == false && ram_disable[1] == false) board.prgram.data[addr] = data;
}

void power() {
  reset();
}

void reset() {
  chr_mode = 0;
  prg_size = 1;
  prg_mode = 1;
  mirror = 0;
  chr_bank[0] = 0;
  chr_bank[1] = 1;
  prg_bank = 0;
  prg_page = 0;
  ram_bank = 0;
  ram_disable[0] = 0;
  ram_disable[1] = 0;
}

void serialize(serializer &s) {
  s.integer(chr_mode);
  s.integer(prg_size);
  s.integer(prg_mode);
  s.integer(mirror);
  s.array(chr_bank);
  s.integer(prg_bank);
  s.integer(prg_page);
  s.integer(ram_bank);
  s.array(ram_disable);
}

MMC1(Board &board) : Chip(board) {
  revision = Revision::MMC1B2;
}

};
