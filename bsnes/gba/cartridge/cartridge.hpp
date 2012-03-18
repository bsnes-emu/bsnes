struct Cartridge : property<Cartridge> {
  readonly<bool> loaded;
  readonly<string> sha256;

  uint8_t *romdata;
  unsigned romsize;

  void load(const string &markup, const uint8_t *data, unsigned size);
  void unload();

  void power();

  Cartridge();
};

extern Cartridge cartridge;
