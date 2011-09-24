struct MMC3 : Mapper {
  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  uint8 ciram_read(uint13 addr);
  void ciram_write(uint13 addr, uint8 data);

  unsigned ram_size();
  uint8* ram_data();

  void power();
  void reset();

  void serialize(serializer&);

private:
  uint8 prg_ram[8192];

  uint8 bank_select;
  uint8 prg_bank[2];
  uint8 chr_bank[6];
  bool mirror_select;
  bool prg_ram_enable;
  bool prg_ram_write_protect;
  uint8 irq_latch;
  uint8 irq_counter;
  bool irq_enable;
  unsigned irq_delay;
  bool irq_line;

  uint16 last_chr_addr;

  unsigned prg_addr(uint16 addr);
  unsigned chr_addr(uint16 addr);
  unsigned ciram_addr(uint13 addr);
  void clock_irq_test(uint16 addr);
};

extern MMC3 mmc3;
