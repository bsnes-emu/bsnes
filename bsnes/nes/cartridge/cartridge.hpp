struct Cartridge : property<Cartridge> {
  void load(const string &xml, const uint8_t *data, unsigned size);
  void unload();

  unsigned ram_size();
  uint8 *ram_data();

  void power();
  void reset();

  readonly<bool> loaded;
  readonly<string> sha256;

  void serialize(serializer&);
  Cartridge();

//privileged:
  Mapper::Mapper *mapper;

  uint8 prg_read(uint16 addr);
  void prg_write(uint16 addr, uint8 data);

  uint8 chr_read(uint16 addr);
  void chr_write(uint16 addr, uint8 data);

  uint8 ciram_read(uint13 addr);
  void ciram_write(uint13 addr, uint8 data);

  uint8 *rom_data;
  unsigned rom_size;

  uint8 *prg_data;
  unsigned prg_size;

  uint8 *chr_data;
  unsigned chr_size;

  bool chr_ram;
  unsigned mirroring;
};

extern Cartridge cartridge;
