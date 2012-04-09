struct Cartridge : property<Cartridge> {
  StaticMemory rom;
  StaticMemory ram;
  #include "memory.hpp"

  readonly<bool> loaded;
  readonly<string> sha256;
  readonly<bool> has_eeprom;

  bool load(const string &markup, const uint8_t *data, unsigned size);
  void unload();
  void power();

  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);

  Cartridge();
};

extern Cartridge cartridge;
