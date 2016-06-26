struct KonamiVRC6 : Board {
  KonamiVRC6(Markup::Node& document) : Board(document), vrc6(*this) {
  }

  auto prg_read(uint addr) -> uint8{
    if((addr & 0xe000) == 0x6000) return vrc6.ram_read(addr);
    if(addr & 0x8000) return prgrom.read(vrc6.prg_addr(addr));
    return cpu.mdr();
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if((addr & 0xe000) == 0x6000) return vrc6.ram_write(addr, data);
    if(addr & 0x8000) {
      addr = (addr & 0xf003);
      if(prgram.size) addr = (addr & ~3) | ((addr & 2) >> 1) | ((addr & 1) << 1);
      return vrc6.reg_write(addr, data);
    }
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(vrc6.ciram_addr(addr));
    return Board::chr_read(vrc6.chr_addr(addr));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(vrc6.ciram_addr(addr), data);
    return Board::chr_write(vrc6.chr_addr(addr), data);
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
