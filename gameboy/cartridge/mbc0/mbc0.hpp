struct MBC0 : MMIO {
  uint8 mmio_read(uint16 addr);
  void mmio_write(uint16 addr, uint8 data);
  void power();
  void reset();
} mbc0;
