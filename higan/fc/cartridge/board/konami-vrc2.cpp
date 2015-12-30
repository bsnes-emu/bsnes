struct KonamiVRC2 : Board {
  KonamiVRC2(Markup::Node& document) : Board(document), vrc2(*this) {
    settings.pinout.a0 = 1 << document["cartridge/chip/pinout/a0"].natural();
    settings.pinout.a1 = 1 << document["cartridge/chip/pinout/a1"].natural();
  }

  auto prg_read(uint addr) -> uint8 {
    if(addr < 0x6000) return cpu.mdr();
    if(addr < 0x8000) return vrc2.ram_read(addr);
    return prgrom.read(vrc2.prg_addr(addr));
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if(addr < 0x6000) return;
    if(addr < 0x8000) return vrc2.ram_write(addr, data);

    bool a0 = (addr & settings.pinout.a0);
    bool a1 = (addr & settings.pinout.a1);
    addr &= 0xfff0;
    addr |= (a0 << 0) | (a1 << 1);
    return vrc2.reg_write(addr, data);
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.ciram_read(vrc2.ciram_addr(addr));
    return Board::chr_read(vrc2.chr_addr(addr));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.ciram_write(vrc2.ciram_addr(addr), data);
    return Board::chr_write(vrc2.chr_addr(addr), data);
  }

  auto power() -> void {
    vrc2.power();
  }

  auto reset() -> void {
    vrc2.reset();
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
