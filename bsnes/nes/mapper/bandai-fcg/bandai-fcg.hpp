struct BandaiFCG : Mapper {
  void main();

  uint8 prg_read(unsigned addr);
  void prg_write(unsigned addr, uint8 data);

  uint8 chr_read(unsigned addr);
  void chr_write(unsigned addr, uint8 data);

  void power();
  void reset();

  void serialize(serializer&);

private:
  unsigned ciram_addr(unsigned addr) const;

  uint8 chr_bank[8];
  uint8 prg_bank;
  uint8 mirror_select;
  bool irq_counter_enable;
  uint16 irq_counter;
  uint16 irq_latch;
};

extern BandaiFCG bandaiFCG;
