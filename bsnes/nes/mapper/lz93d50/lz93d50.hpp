struct LZ93D50 : Mapper {
  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  uint8 ciram_read(uint13 addr);
  void ciram_write(uint13 addr, uint8 data);

  void power();
  void reset();

private:
  unsigned ciram_addr(unsigned addr) const;
  void clock();

  uint8 chr_bank[8];
  uint8 prg_bank;
  uint8 mirror_select;
  bool irq_counter_enable;
  uint16 irq_counter;
  uint16 irq_latch;
};

extern LZ93D50 lz93d50;
