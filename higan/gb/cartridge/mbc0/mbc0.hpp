struct MBC0 : MMIO {
  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;
  auto power() -> void;
} mbc0;
