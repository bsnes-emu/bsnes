struct KonamiVRC1 : Board {
  KonamiVRC1(Markup::Node& document) : Board(document), vrc1(*this) {
  }

  auto readPRG(uint addr) -> uint8 {
    if(addr & 0x8000) return prgrom.read(vrc1.addrPRG(addr));
    return cpu.mdr();
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if(addr & 0x8000) return vrc1.writeIO(addr, data);
  }

  auto readCHR(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(vrc1.addrCIRAM(addr));
    return Board::readCHR(vrc1.addrCHR(addr));
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(vrc1.addrCIRAM(addr), data);
    return Board::writeCHR(vrc1.addrCHR(addr), data);
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
