struct MMC1 : Mapper {
  uint8 prg_read(unsigned addr);
  void prg_write(unsigned addr, uint8 data);

  uint8 chr_read(unsigned addr);
  void chr_write(unsigned addr, uint8 data);

  unsigned ram_size();
  uint8* ram_data();

  void power();
  void reset();

  void serialize(serializer&);

private:
  uint8 prg_ram[8192];
  uint8 r[4];
  bool prg_ex_select;
  unsigned shiftaddr;
  unsigned shiftdata;

  unsigned ciram_addr(unsigned addr) const;
  bool prg_mode() const;
  bool prg_size() const;
  unsigned prg_addr(bool region) const;
  bool chr_mode() const;
  unsigned chr_banklo() const;
  unsigned chr_bankhi() const;
  unsigned prg_bank() const;
  bool prg_ram_disable() const;
};

extern MMC1 mmc1;
