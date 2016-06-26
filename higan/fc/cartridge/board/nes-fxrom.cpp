//MMC4

struct NES_FxROM : Board {
  NES_FxROM(Markup::Node& document) : Board(document) {
    revision = Revision::FKROM;
  }

  auto prg_read(uint addr) -> uint8 {
    if(addr < 0x6000) return cpu.mdr();
    if(addr < 0x8000) return prgram.read(addr);
    uint bank = addr < 0xc000 ? prg_bank : (uint4)0x0f;
    return prgrom.read((bank * 0x4000) | (addr & 0x3fff));
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if(addr < 0x6000) return;
    if(addr < 0x8000) return prgram.write(addr, data);

    switch(addr & 0xf000) {
    case 0xa000: prg_bank = data & 0x0f; break;
    case 0xb000: chr_bank[0][0] = data & 0x1f; break;
    case 0xc000: chr_bank[0][1] = data & 0x1f; break;
    case 0xd000: chr_bank[1][0] = data & 0x1f; break;
    case 0xe000: chr_bank[1][1] = data & 0x1f; break;
    case 0xf000: mirror = data & 0x01; break;
    }
  }

  auto ciram_addr(uint addr) const -> uint {
    switch(mirror) {
    case 0: return ((addr & 0x0400) >> 0) | (addr & 0x03ff);  //vertical mirroring
    case 1: return ((addr & 0x0800) >> 1) | (addr & 0x03ff);  //horizontal mirroring
    }
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(ciram_addr(addr));
    bool region = addr & 0x1000;
    uint bank = chr_bank[region][latch[region]];
    if((addr & 0x0ff8) == 0x0fd8) latch[region] = 0;
    if((addr & 0x0ff8) == 0x0fe8) latch[region] = 1;
    return Board::chr_read((bank * 0x1000) | (addr & 0x0fff));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(ciram_addr(addr), data);
    bool region = addr & 0x1000;
    uint bank = chr_bank[region][latch[region]];
    if((addr & 0x0ff8) == 0x0fd8) latch[region] = 0;
    if((addr & 0x0ff8) == 0x0fe8) latch[region] = 1;
    return Board::chr_write((bank * 0x1000) | (addr & 0x0fff), data);
  }

  auto power() -> void {
  }

  auto reset() -> void {
    prg_bank = 0;
    chr_bank[0][0] = 0;
    chr_bank[0][1] = 0;
    chr_bank[1][0] = 0;
    chr_bank[1][1] = 0;
    mirror = 0;
    latch[0] = 0;
    latch[1] = 0;
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);

    s.integer(prg_bank);
    s.integer(chr_bank[0][0]);
    s.integer(chr_bank[0][1]);
    s.integer(chr_bank[1][0]);
    s.integer(chr_bank[1][1]);
    s.integer(mirror);
    s.array(latch);
  }

  enum Revision : uint {
    FJROM,
    FKROM,
  } revision;

  uint4 prg_bank;
  uint5 chr_bank[2][2];
  bool mirror;
  bool latch[2];
};
