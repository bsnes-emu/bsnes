struct Cartridge {
  auto pathID() const -> uint { return information.pathID; }
  auto region() const -> string { return information.region; }
  auto hash() const -> string { return information.sha256; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;

  //mapper.cpp
  auto read(uint16 addr) -> maybe<uint8>;
  auto write(uint16 addr, uint8 data) -> bool;

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

  struct Memory {
    uint8* data = nullptr;
    uint size = 0;
    uint mask = 0;

    static auto mirror(uint addr, uint size) -> uint;
    auto read(uint addr) -> uint8;
    auto write(uint addr, uint8 data) -> void;
  };

  Memory rom;
  Memory ram;

  struct Mapper {
    //$fffc
    uint2 shift;
    uint1 ramPage2;
    uint1 ramEnablePage2;
    uint1 ramEnablePage3;
    uint1 romWriteEnable;

    //$fffd
    uint8 romPage0;

    //$fffe
    uint8 romPage1;

    //$ffff
    uint8 romPage2;
  } mapper;
};

extern Cartridge cartridge;
