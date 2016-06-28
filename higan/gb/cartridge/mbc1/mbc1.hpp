struct MBC1 : MMIO {
  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  struct ROM {
    uint8 select;
  } rom;
  struct RAM {
    bool enable;
    uint8 select;
  } ram;
  bool mode;
} mbc1;
