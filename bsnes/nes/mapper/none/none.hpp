struct None : Mapper {
  uint8 prg_read(unsigned addr);
  void prg_write(unsigned addr, uint8 data);

  uint8 chr_read(unsigned addr);
  void chr_write(unsigned addr, uint8 data);

  void power();
  void reset();

  void serialize(serializer&);
};

extern None none;
