struct Cartridge {
  #include "memory.hpp"

  auto loaded() const -> bool;
  auto sha256() const -> string;
  auto title() const -> string;

  struct Information {
    string markup;
    string sha256;
    string title;
  } information;

  struct Media {
    uint id;
    string name;
  };
  vector<Media> memory;

  Cartridge();
  ~Cartridge();

  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto read(uint mode, uint32 addr) -> uint32;
  auto write(uint mode, uint32 addr, uint32 word) -> void;

  auto serialize(serializer&) -> void;

private:
  bool isLoaded = false;
  bool hasSRAM = false;
  bool hasEEPROM = false;
  bool hasFLASH = false;
};

extern Cartridge cartridge;
