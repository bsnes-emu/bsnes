class Cartridge : property<Cartridge> {
public:
  struct Information {
    string   name;
    uint8    cgbflag;
    uint8    sgbflag;
    uint8    type;
    unsigned romsize;
    unsigned ramsize;
  } info;

  readonly<bool> loaded;

  void load(uint8_t *data, unsigned size);
  void unload();
  Cartridge();
};

extern Cartridge cartridge;
