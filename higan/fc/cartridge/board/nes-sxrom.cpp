struct NES_SxROM : Board {
  NES_SxROM(Markup::Node& document) : Board(document), mmc1(*this) {
    revision = Revision::SXROM;
  }

  auto main() -> void {
    return mmc1.main();
  }

  auto addrRAM(uint addr) -> uint {
    uint bank = 0;
    if(revision == Revision::SOROM) bank = (mmc1.chrBank[0] & 0x08) >> 3;
    if(revision == Revision::SUROM) bank = (mmc1.chrBank[0] & 0x0c) >> 2;
    if(revision == Revision::SXROM) bank = (mmc1.chrBank[0] & 0x0c) >> 2;
    return (bank << 13) | (addr & 0x1fff);
  }

  auto readPRG(uint addr) -> uint8 {
    if((addr & 0xe000) == 0x6000) {
      if(revision == Revision::SNROM) {
        if(mmc1.chrBank[0] & 0x10) return cpu.mdr();
      }
      if(mmc1.ramDisable) return 0x00;
      return prgram.read(addrRAM(addr));
    }

    if(addr & 0x8000) {
      addr = mmc1.addrPRG(addr);
      if(revision == Revision::SXROM) {
        addr |= ((mmc1.chrBank[0] & 0x10) >> 4) << 18;
      }
      return prgrom.read(addr);
    }

    return cpu.mdr();
  }

  auto writePRG(uint addr, uint8 data) -> void {
    if((addr & 0xe000) == 0x6000) {
      if(revision == Revision::SNROM) {
        if(mmc1.chrBank[0] & 0x10) return;
      }
      if(mmc1.ramDisable) return;
      return prgram.write(addrRAM(addr), data);
    }

    if(addr & 0x8000) return mmc1.writeIO(addr, data);
  }

  auto readCHR(uint addr) -> uint8 {
    if(addr & 0x2000) return ppu.readCIRAM(mmc1.addrCIRAM(addr));
    return Board::readCHR(mmc1.addrCHR(addr));
  }

  auto writeCHR(uint addr, uint8 data) -> void {
    if(addr & 0x2000) return ppu.writeCIRAM(mmc1.addrCIRAM(addr), data);
    return Board::writeCHR(mmc1.addrCHR(addr), data);
  }

  auto power() -> void {
    mmc1.power();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    mmc1.serialize(s);
  }

  enum class Revision : uint {
    SAROM,
    SBROM,
    SCROM,
    SC1ROM,
    SEROM,
    SFROM,
    SGROM,
    SHROM,
    SH1ROM,
    SIROM,
    SJROM,
    SKROM,
    SLROM,
    SL1ROM,
    SL2ROM,
    SL3ROM,
    SLRROM,
    SMROM,
    SNROM,
    SOROM,
    SUROM,
    SXROM,
  } revision;

  MMC1 mmc1;
};
