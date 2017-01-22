struct KonamiVRC4 : Board {
  KonamiVRC4(Markup::Node& document) : Board(document), vrc4(*this) {
    settings.pinout.a0 = 1 << document["board/chip/pinout/a0"].natural();
    settings.pinout.a1 = 1 << document["board/chip/pinout/a1"].natural();
  }

  auto main() -> void {
    return vrc4.main();
  }

  auto readPRG(uint addr) -> uint8 {
    if(addr < 0x6000) return cpu.mdr();
    if(addr < 0x8000) return prgram.read(addr);
    return prgrom.read(vrc4.addrPRG(addr));
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if(addr < 0x6000) return;
    if(addr < 0x8000) return prgram.write(addr, data);

    bool a0 = (addr & settings.pinout.a0);
    bool a1 = (addr & settings.pinout.a1);
    addr &= 0xfff0;
    addr |= (a1 << 1) | (a0 << 0);
    return vrc4.writeIO(addr, data);
  }

  auto readCHR(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(vrc4.addrCIRAM(addr));
    return Board::readCHR(vrc4.addrCHR(addr));
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(vrc4.addrCIRAM(addr), data);
    return Board::writeCHR(vrc4.addrCHR(addr), data);
  }

  auto power() -> void {
    vrc4.power();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    vrc4.serialize(s);
  }

  struct Settings {
    struct Pinout {
      uint a0;
      uint a1;
    } pinout;
  } settings;

  VRC4 vrc4;
};
