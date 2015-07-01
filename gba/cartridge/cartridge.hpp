struct Cartridge : property<Cartridge> {
  #include "memory.hpp"

  readonly<bool> loaded;
  readonly<string> sha256;

  readonly<bool> has_sram;
  readonly<bool> has_eeprom;
  readonly<bool> has_flashrom;

  struct Information {
    string markup;
    string title;
  } information;

  string title();

  struct Media {
    unsigned id;
    string name;
  };
  vector<Media> memory;

  void load();
  void unload();
  void power();

  uint8* ram_data();
  unsigned ram_size();

  auto read(uint8* data, unsigned mode, uint32 addr) -> uint32;
  auto write(uint8* data, unsigned mode, uint32 addr, uint32 word) -> void;

  auto rom_read(unsigned mode, uint32 addr) -> uint32;
  auto rom_write(unsigned mode, uint32 addr, uint32 word) -> void;

  auto ram_read(unsigned mode, uint32 addr) -> uint32;
  auto ram_write(unsigned mode, uint32 addr, uint32 word) -> void;

  void serialize(serializer&);

  Cartridge();
  ~Cartridge();
};

extern Cartridge cartridge;
