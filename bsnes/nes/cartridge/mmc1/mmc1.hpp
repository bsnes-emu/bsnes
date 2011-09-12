struct MMC1 : Mapper {
  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  void power();
  void reset();

private:
  uint8 prg_ram[8192];
  uint8 r[4];
  unsigned shiftaddr;
  unsigned shiftdata;

  unsigned ciram_addr(unsigned addr) const;
  bool prg_mode() const;
  unsigned prg_addr() const;
  bool chr_mode() const;
  unsigned chr_banklo() const;
  unsigned chr_bankhi() const;
  unsigned prg_bank() const;
  bool prg_ram_disable() const;

  uint8& prg_data(unsigned addr);
  uint8& chr_data(unsigned addr);
};
