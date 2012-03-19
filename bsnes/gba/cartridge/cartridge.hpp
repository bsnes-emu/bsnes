struct Cartridge : property<Cartridge> {
  StaticMemory rom;
  StaticMemory ram;

  readonly<bool> loaded;
  readonly<string> sha256;

  bool load(const string &markup, const uint8_t *data, unsigned size);
  void unload();

  Cartridge();
};

extern Cartridge cartridge;
