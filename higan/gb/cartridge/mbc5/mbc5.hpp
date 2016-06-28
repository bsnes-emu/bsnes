struct MBC5 : MMIO {
  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  struct ROM {
    uint9 select;
  } rom;
  struct RAM {
    bool enable;
    uint4 select;
  } ram;
} mbc5;
