struct KonamiVRC7 : Board {
  KonamiVRC7(Markup::Node& document) : Board(document), vrc7(*this) {
  }

  auto main() -> void {
    return vrc7.main();
  }

  auto prg_read(uint addr) -> uint8 {
    if(addr < 0x6000) return cpu.mdr();
    if(addr < 0x8000) return prgram.read(addr);
    return prgrom.read(vrc7.prg_addr(addr));
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if(addr < 0x6000) return;
    if(addr < 0x8000) return prgram.write(addr, data);
    return vrc7.reg_write(addr, data);
  }

  auto chr_read(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.ciram_read(vrc7.ciram_addr(addr));
    return chrram.read(vrc7.chr_addr(addr));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.ciram_write(vrc7.ciram_addr(addr), data);
    return chrram.write(vrc7.chr_addr(addr), data);
  }

  auto power() -> void {
    vrc7.power();
  }

  auto reset() -> void {
    vrc7.reset();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    vrc7.serialize(s);
  }

  VRC7 vrc7;
};
