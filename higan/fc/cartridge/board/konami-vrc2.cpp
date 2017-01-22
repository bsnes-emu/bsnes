struct KonamiVRC2 : Board {
  KonamiVRC2(Markup::Node& document) : Board(document), vrc2(*this) {
    settings.pinout.a0 = 1 << document["board/chip/pinout/a0"].natural();
    settings.pinout.a1 = 1 << document["board/chip/pinout/a1"].natural();
  }

  auto readPRG(uint addr) -> uint8 {
    if(addr < 0x6000) return cpu.mdr();
    if(addr < 0x8000) return vrc2.readRAM(addr);
    return prgrom.read(vrc2.addrPRG(addr));
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if(addr < 0x6000) return;
    if(addr < 0x8000) return vrc2.writeRAM(addr, data);

    bool a0 = (addr & settings.pinout.a0);
    bool a1 = (addr & settings.pinout.a1);
    addr &= 0xfff0;
    addr |= (a0 << 0) | (a1 << 1);
    return vrc2.writeIO(addr, data);
  }

  auto readCHR(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(vrc2.addrCIRAM(addr));
    return Board::readCHR(vrc2.addrCHR(addr));
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(vrc2.addrCIRAM(addr), data);
    return Board::writeCHR(vrc2.addrCHR(addr), data);
  }

  auto power() -> void {
    vrc2.power();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    vrc2.serialize(s);
  }

  struct Settings {
    struct Pinout {
      uint a0;
      uint a1;
    } pinout;
  } settings;

  VRC2 vrc2;
};
