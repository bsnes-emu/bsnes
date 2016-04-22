struct MBC1M : MMIO {
  auto mmio_read(uint16 addr) -> uint8;
  auto mmio_write(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  uint4 romLo;
  uint2 romHi;
} mbc1m;
