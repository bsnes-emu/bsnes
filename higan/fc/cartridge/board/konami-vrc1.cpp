struct KonamiVRC1 : Board {
  KonamiVRC1(Markup::Node& document) : Board(document), vrc1(*this) {
  }

  auto prg_read(uint addr) -> uint8 {
    if(addr & 0x8000) return prgrom.read(vrc1.prg_addr(addr));
    return cpu.mdr();
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if(addr & 0x8000) return vrc1.reg_write(addr, data);
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(vrc1.ciram_addr(addr));
    return Board::chr_read(vrc1.chr_addr(addr));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(vrc1.ciram_addr(addr), data);
    return Board::chr_write(vrc1.chr_addr(addr), data);
  }

  auto power() -> void {
    vrc1.power();
  }

  auto reset() -> void {
    vrc1.reset();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    vrc1.serialize(s);
  }

  VRC1 vrc1;
};
