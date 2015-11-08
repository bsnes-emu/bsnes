struct Cartridge : property<Cartridge> {
  #include "memory.hpp"

  readonly<bool> loaded;
  readonly<string> sha256;

  readonly<bool> has_sram;
  readonly<bool> has_eeprom;
  readonly<bool> has_flash;

  struct Information {
    string markup;
    string title;
  } information;

  struct Media {
    unsigned id;
    string name;
  };
  vector<Media> memory;

  Cartridge();
  ~Cartridge();

  auto title() const -> string;

  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto read(unsigned mode, uint32 addr) -> uint32;
  auto write(unsigned mode, uint32 addr, uint32 word) -> void;

  auto serialize(serializer&) -> void;
};

extern Cartridge cartridge;
