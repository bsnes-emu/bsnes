struct Cartridge : MMIO, property<Cartridge> {
  #include "mmio/mmio.hpp"

  struct Information {
    string   name;
    uint8    cgbflag;
    uint8    sgbflag;
    uint8    type;
    unsigned romsize;
    unsigned ramsize;
  } info;

  readonly<bool> loaded;

  uint8_t *romdata;
  unsigned romsize;

  uint8_t *ramdata;
  unsigned ramsize;

  void load(uint8_t *data, unsigned size);
  void unload();

  void power();
  void reset();

  Cartridge();
  ~Cartridge();
};

extern Cartridge cartridge;
