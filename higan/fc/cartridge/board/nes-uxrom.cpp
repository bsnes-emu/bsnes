//NES-UNROM
//NES-UOROM

struct NES_UxROM : Board {
  NES_UxROM(Markup::Node& document) : Board(document) {
    settings.mirror = document["board/mirror/mode"].text() == "vertical" ? 1 : 0;
  }

  auto prg_read(uint addr) -> uint8 {
    if((addr & 0xc000) == 0x8000) return prgrom.read((prg_bank << 14) | (addr & 0x3fff));
    if((addr & 0xc000) == 0xc000) return prgrom.read((    0x0f << 14) | (addr & 0x3fff));
    return cpu.mdr();
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if(addr & 0x8000) prg_bank = data & 0x0f;
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) {
      if(settings.mirror == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
      return ppu.readCIRAM(addr);
    }
    return Board::chr_read(addr);
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) {
      if(settings.mirror == 0) addr = ((addr & 0x0800) >> 1) | (addr & 0x03ff);
      return ppu.writeCIRAM(addr, data);
    }
    return Board::chr_write(addr, data);
  }

  auto power() -> void {
  }

  auto reset() -> void {
    prg_bank = 0;
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);

    s.integer(prg_bank);
  }

  struct Settings {
    bool mirror;  //0 = horizontal, 1 = vertical
  } settings;

  uint4 prg_bank;
};
