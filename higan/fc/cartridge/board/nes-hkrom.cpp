struct NES_HKROM : Board {
  NES_HKROM(Markup::Node& document) : Board(document), mmc6(*this) {
  }

  auto main() -> void {
    mmc6.main();
  }

  auto prg_read(uint addr) -> uint8 {
    if((addr & 0xf000) == 0x7000) return mmc6.ram_read(addr);
    if(addr & 0x8000) return prgrom.read(mmc6.prg_addr(addr));
    return cpu.mdr();
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if((addr & 0xf000) == 0x7000) return mmc6.ram_write(addr, data);
    if(addr & 0x8000) return mmc6.reg_write(addr, data);
  }

  auto chr_read(uint addr) -> uint8 {
    mmc6.irq_test(addr);
    if(addr & 0x2000) return ppu.readCIRAM(mmc6.ciram_addr(addr));
    return Board::chr_read(mmc6.chr_addr(addr));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    mmc6.irq_test(addr);
    if(addr & 0x2000) return ppu.writeCIRAM(mmc6.ciram_addr(addr), data);
    return Board::chr_write(mmc6.chr_addr(addr), data);
  }

  auto power() -> void {
    mmc6.power();
  }

  auto reset() -> void {
    mmc6.reset();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    mmc6.serialize(s);
  }

  MMC6 mmc6;
};
