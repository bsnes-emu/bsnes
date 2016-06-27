struct NES_TxROM : Board {
  NES_TxROM(Markup::Node& document) : Board(document), mmc3(*this) {
    revision = Revision::TLROM;
  }

  auto main() -> void {
    mmc3.main();
  }

  auto readPRG(uint addr) -> uint8 {
    if((addr & 0xe000) == 0x6000) return mmc3.readRAM(addr);
    if(addr & 0x8000) return prgrom.read(mmc3.addrPRG(addr));
    return cpu.mdr();
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if((addr & 0xe000) == 0x6000) return mmc3.writeRAM(addr, data);
    if(addr & 0x8000) return mmc3.writeIO(addr, data);
  }

  auto readCHR(uint addr) -> uint8 {
    mmc3.irqTest(addr);
    if(addr & 0x2000) return ppu.readCIRAM(mmc3.addrCIRAM(addr));
    return Board::readCHR(mmc3.addrCHR(addr));
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    mmc3.irqTest(addr);
    if(addr & 0x2000) return ppu.writeCIRAM(mmc3.addrCIRAM(addr), data);
    return Board::writeCHR(mmc3.addrCHR(addr), data);
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
