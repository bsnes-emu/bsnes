struct Board {
  unsigned mirror(unsigned addr, unsigned size) const;

  virtual uint8 prg_read(unsigned addr);
  virtual void prg_write(unsigned addr, uint8 data);

  virtual uint8 chr_read(unsigned addr);
  virtual void chr_write(unsigned addr, uint8 data);

  virtual void configure(const string &xml) = 0;

  static Board* create(const string &xml, const uint8_t *data, unsigned size);

  struct Information {
    string type;
  } information;

  struct Memory {
    uint8_t *data;
    unsigned size;
  };

  Memory prg;
  Memory chr;
};
