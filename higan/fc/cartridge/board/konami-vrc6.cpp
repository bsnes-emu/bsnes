struct KonamiVRC6 : Board {
  KonamiVRC6(Markup::Node& document) : Board(document), vrc6(*this) {
  }

  auto readPRG(uint addr) -> uint8{
    if((addr & 0xe000) == 0x6000) return vrc6.readRAM(addr);
    if(addr & 0x8000) return prgrom.read(vrc6.addrPRG(addr));
    return cpu.mdr();
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if((addr & 0xe000) == 0x6000) return vrc6.writeRAM(addr, data);
    if(addr & 0x8000) {
      addr = (addr & 0xf003);
      if(prgram.size) addr = (addr & ~3) | ((addr & 2) >> 1) | ((addr & 1) << 1);
      return vrc6.writeIO(addr, data);
    }
  }

  auto readCHR(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(vrc6.addrCIRAM(addr));
    return Board::readCHR(vrc6.addrCHR(addr));
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(vrc6.addrCIRAM(addr), data);
    return Board::writeCHR(vrc6.addrCHR(addr), data);
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    vrc6.serialize(s);
  }

  auto main() -> void { vrc6.main(); }
  auto power() -> void { vrc6.power(); }
  auto reset() -> void { vrc6.reset(); }

  VRC6 vrc6;
};
