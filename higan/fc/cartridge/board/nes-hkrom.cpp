struct NES_HKROM : Board {
  NES_HKROM(Markup::Node& document) : Board(document), mmc6(*this) {
  }

  auto main() -> void {
    mmc6.main();
  }

  auto readPRG(uint addr) -> uint8 {
    if((addr & 0xf000) == 0x7000) return mmc6.readRAM(addr);
    if(addr & 0x8000) return prgrom.read(mmc6.addrPRG(addr));
    return cpu.mdr();
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if((addr & 0xf000) == 0x7000) return mmc6.writeRAM(addr, data);
    if(addr & 0x8000) return mmc6.writeIO(addr, data);
  }

  auto readCHR(uint addr) -> uint8 {
    mmc6.irqTest(addr);
    if(addr & 0x2000) return ppu.readCIRAM(mmc6.addrCIRAM(addr));
    return Board::readCHR(mmc6.addrCHR(addr));
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    mmc6.irqTest(addr);
    if(addr & 0x2000) return ppu.writeCIRAM(mmc6.addrCIRAM(addr), data);
    return Board::writeCHR(mmc6.addrCHR(addr), data);
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
