struct Cartridge : property<Cartridge> {
  void load(const string &xml, const uint8_t *data, unsigned size);
  void unload();

  readonly<bool> loaded;

  Cartridge();

  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  uint8 *prg_data;
  unsigned prg_size;

  uint8 *chr_data;
  unsigned chr_size;

  bool chr_ram;
};

extern Cartridge cartridge;
