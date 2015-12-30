struct NES_TxROM : Board {
  NES_TxROM(Markup::Node& document) : Board(document), mmc3(*this) {
    revision = Revision::TLROM;
  }

  auto main() -> void {
    mmc3.main();
  }

  auto prg_read(uint addr) -> uint8 {
    if((addr & 0xe000) == 0x6000) return mmc3.ram_read(addr);
    if(addr & 0x8000) return prgrom.read(mmc3.prg_addr(addr));
    return cpu.mdr();
  }

  auto prg_write(uint addr, uint8 data) -> void {
    if((addr & 0xe000) == 0x6000) return mmc3.ram_write(addr, data);
    if(addr & 0x8000) return mmc3.reg_write(addr, data);
  }

  auto chr_read(uint addr) -> uint8 {
    mmc3.irq_test(addr);
    if(addr & 0x2000) return ppu.ciram_read(mmc3.ciram_addr(addr));
    return Board::chr_read(mmc3.chr_addr(addr));
  }

  auto chr_write(uint addr, uint8 data) -> void {
    mmc3.irq_test(addr);
    if(addr & 0x2000) return ppu.ciram_write(mmc3.ciram_addr(addr), data);
    return Board::chr_write(mmc3.chr_addr(addr), data);
  }

  auto power() -> void {
    mmc3.power();
  }

  auto reset() -> void {
    mmc3.reset();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    mmc3.serialize(s);
  }

  enum class Revision : uint {
    TBROM,
    TEROM,
    TFROM,
    TGROM,
    TKROM,
    TKSROM,
    TLROM,
    TL1ROM,
    TL2ROM,
    TLSROM,
    TNROM,
    TQROM,
    TR1ROM,
    TSROM,
    TVROM,
  } revision;

  MMC3 mmc3;
};
