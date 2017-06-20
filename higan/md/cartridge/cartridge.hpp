struct Cartridge {
  auto pathID() const -> uint { return information.pathID; }
  auto region() const -> string { return information.region; }
  auto sha256() const -> string { return information.sha256; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto read(uint24 addr) -> uint16;
  auto write(uint24 addr, uint16 data) -> void;

  auto readIO(uint24 addr) -> uint16;
  auto writeIO(uint24 addr, uint16 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Information {
    uint pathID = 0;
    string region;
    string sha256;
    string manifest;
    string title;
  } information;

  struct Memory {
    uint8* data = nullptr;
    uint size = 0;
    uint mask = 0;
  };

  Memory rom;
  Memory ram;

  uint1 ramEnable;
  uint1 ramWritable;
  uint6 bank[8];
};

extern Cartridge cartridge;
