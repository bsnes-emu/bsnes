struct Cartridge {
  auto pathID() const -> uint { return information.pathID; }
  auto region() const -> string { return information.region; }
  auto hash() const -> string { return information.sha256; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  //cartridge.cpp
  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;

  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct Information {
    uint pathID = 0;
    string region;
    string sha256;
    string manifest;
    string title;
  } information;

  Emulator::Memory::Readable<uint8> rom;
  Emulator::Memory::Writable<uint8> ram;
};

extern Cartridge cartridge;
extern Cartridge expansion;
