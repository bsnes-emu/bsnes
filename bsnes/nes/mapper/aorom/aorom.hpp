struct AOROM : Mapper {
  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  uint8 ciram_read(uint13 addr);
  void ciram_write(uint13 addr, uint8 data);

  void power();
  void reset();

private:
  uint4 prg_bank;
  bool mirror_select;
};

extern AOROM aorom;
