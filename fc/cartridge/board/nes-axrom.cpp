//NES-AMROM
//NES-ANROM
//NES-AN1ROM
//NES-AOROM

struct NES_AxROM : Board {
  NES_AxROM(Markup::Node& document) : Board(document) {
  }

  auto prg_read(uint addr) -> uint8 {
    if(addr & 0x8000) return prgrom.read((prg_bank << 15) | (addr & 0x7fff));
    return cpu.mdr();
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if(addr & 0x8000) {
      prg_bank = data & 0x0f;
      mirror_select = data & 0x10;
    }
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.ciram_read((mirror_select << 10) | (addr & 0x03ff));
    return Board::chr_read(addr);
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.ciram_write((mirror_select << 10) | (addr & 0x03ff), data);
    return Board::chr_write(addr, data);
  }

  auto power() -> void {
  }

  auto reset() -> void {
    prg_bank = 0x0f;
    mirror_select = 0;
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);

    s.integer(prg_bank);
    s.integer(mirror_select);
  }

  uint4 prg_bank;
  bool mirror_select;
};
