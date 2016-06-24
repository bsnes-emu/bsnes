struct Cartridge {
  #include "memory.hpp"

  auto sha256() const -> string { return information.sha256; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  struct Information {
    string sha256;
    string manifest;
    string title;
  } information;

  Cartridge();
  ~Cartridge();

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto read(uint mode, uint32 addr) -> uint32;
  auto write(uint mode, uint32 addr, uint32 word) -> void;

  auto serialize(serializer&) -> void;

private:
  bool hasSRAM = false;
  bool hasEEPROM = false;
  bool hasFLASH = false;
};

extern Cartridge cartridge;
