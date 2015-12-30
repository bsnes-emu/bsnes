struct MBC2 : MMIO {
  auto mmio_read(uint16 addr) -> uint8;
  auto mmio_write(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  bool  ram_enable;  //$0000-1fff
  uint8 rom_select;  //$2000-3fff
} mbc2;
