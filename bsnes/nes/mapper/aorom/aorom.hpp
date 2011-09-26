struct AOROM : Mapper {
  uint8 prg_read(unsigned addr);
  void prg_write(unsigned addr, uint8 data);

  uint8 chr_read(unsigned addr);
  void chr_write(unsigned addr, uint8 data);

  void power();
  void reset();

  void serialize(serializer&);

private:
  uint4 prg_bank;
  bool mirror_select;
};

extern AOROM aorom;
