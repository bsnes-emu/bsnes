struct MMC1 : Mapper {
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

  uint8& prg_data(unsigned addr);
  uint8& chr_data(unsigned addr);
};

extern MMC1 mmc1;
