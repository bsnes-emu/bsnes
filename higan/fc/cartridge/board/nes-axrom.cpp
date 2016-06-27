//NES-AMROM
//NES-ANROM
//NES-AN1ROM
//NES-AOROM

struct NES_AxROM : Board {
  NES_AxROM(Markup::Node& document) : Board(document) {
  }

  auto readPRG(uint addr) -> uint8 {
    if(addr & 0x8000) return prgrom.read((prgBank << 15) | (addr & 0x7fff));
    return cpu.mdr();
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if(addr & 0x8000) {
      prgBank = data & 0x0f;
      mirrorSelect = data & 0x10;
    }
  }

  auto readCHR(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM((mirrorSelect << 10) | (addr & 0x03ff));
    return Board::readCHR(addr);
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM((mirrorSelect << 10) | (addr & 0x03ff), data);
    return Board::writeCHR(addr, data);
  }

  auto power() -> void {
  }

  auto reset() -> void {
    prgBank = 0x0f;
    mirrorSelect = 0;
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);

    s.integer(prgBank);
    s.integer(mirrorSelect);
  }

  uint4 prgBank;
  bool mirrorSelect;
};
