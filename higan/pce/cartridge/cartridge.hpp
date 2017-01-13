struct Cartridge {
  auto pathID() const -> uint { return information.pathID; }
  auto sha256() const -> string { return information.sha256; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;

  auto read(uint20 addr) -> uint8;
  auto write(uint20 addr, uint8 data) -> void;

private:
  auto mirror(uint addr, uint size) -> uint;

  struct Information {
    uint pathID = 0;
    string sha256;
    string manifest;
    string title;
  } information;

  struct Memory {
    uint8* data = nullptr;
    uint size = 0;
  };

  Memory rom;
};

extern Cartridge cartridge;
