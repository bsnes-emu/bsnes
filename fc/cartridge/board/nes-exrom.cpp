struct NES_ExROM : Board {
  NES_ExROM(Markup::Node& document) : Board(document), mmc5(*this) {
    revision = Revision::ELROM;
  }

  auto main() -> void {
    mmc5.main();
  }

  auto prg_read(uint addr) -> uint8 {
    return mmc5.prg_read(addr);
  }

  auto prg_write(uint addr, uint8 data) -> void {
    mmc5.prg_write(addr, data);
  }

  auto chr_read(uint addr) -> uint8 {
    return mmc5.chr_read(addr);
  }

  auto chr_write(uint addr, uint8 data) -> void {
    mmc5.chr_write(addr, data);
  }

  auto scanline(uint y) -> void {
    mmc5.scanline(y);
  }

  auto power() -> void {
    mmc5.power();
  }

  auto reset() -> void {
    mmc5.reset();
  }

  auto serialize(serializer& s) -> void {
    Board::serialize(s);
    mmc5.serialize(s);
  }

  enum class Revision : uint {
    EKROM,
    ELROM,
    ETROM,
    EWROM,
  } revision;

  MMC5 mmc5;
};
