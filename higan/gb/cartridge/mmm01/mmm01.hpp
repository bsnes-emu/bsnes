struct MMM01 : MMIO {
  auto mmio_read(uint16 addr) -> uint8;
  auto mmio_write(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  bool rom_mode;
  uint8 rom_base;

  bool ram_enable;
  uint8 rom_select;
  uint8 ram_select;
} mmm01;
