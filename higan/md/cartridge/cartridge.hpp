struct Cartridge {
  auto pathID() const -> uint { return information.pathID; }
  auto sha256() const -> string { return information.sha256; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto readByte(uint24 addr) -> uint8;
  auto readWord(uint24 addr) -> uint16;
  auto writeByte(uint24 addr, uint8 data) -> void;
  auto writeWord(uint24 addr, uint16 data) -> void;

  struct Information {
    uint pathID = 0;
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
};

extern Cartridge cartridge;
