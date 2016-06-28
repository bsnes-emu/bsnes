struct MBC1M : MMIO {
  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  struct ROM {
    uint4 lo;
    uint2 hi;
  } rom;
  uint1 mode;
} mbc1m;
