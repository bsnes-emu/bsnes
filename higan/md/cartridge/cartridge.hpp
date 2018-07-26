struct Cartridge {
  auto pathID() const -> uint { return information.pathID; }
  auto region() const -> string { return information.region; }

  Cartridge() = default;
  Cartridge(uint depth) : depth(depth) {}

  auto hashes() const -> vector<string>;
  auto manifests() const -> vector<string>;
  auto titles() const -> vector<string>;

  struct Memory;
  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto loadROM(Memory& rom, Markup::Node memory) -> bool;
  auto loadRAM(Memory& ram, Markup::Node memory) -> bool;
  auto saveRAM(Memory& ram, Markup::Node memory) -> bool;

  auto readIO(uint24 address) -> uint16;
  auto writeIO(uint24 address, uint16 data) -> void;

  auto readLinear(uint22 address) -> uint16;
  auto writeLinear(uint22 address, uint16 data) -> void;

  auto readBanked(uint22 address) -> uint16;
  auto writeBanked(uint22 address, uint16 data) -> void;

  auto readLockOn(uint22 address) -> uint16;
  auto writeLockOn(uint22 address, uint16 data) -> void;

  auto readGameGenie(uint22 address) -> uint16;
  auto writeGameGenie(uint22 address, uint16 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Information {
    uint pathID = 0;
    string region;
    string hash;
    string manifest;
    string title;
    Markup::Node document;
  };

  struct Memory {
    explicit operator bool() const;
    auto reset() -> void;
    auto read(uint24 address) -> uint16;
    auto write(uint24 address, uint16 word) -> void;

    uint16* data = nullptr;
    uint size = 0;  //16-bit word size
    uint mask = 0;
    uint bits = 0;
  };

  Information information;

  Memory rom;
  Memory patch;
  Memory ram;

  uint1 ramEnable;
  uint1 ramWritable;
  uint6 romBank[8];

  struct GameGenie {
    boolean enable;
    struct Code {
      boolean enable;
      uint24 address;
      uint16 data;
    } codes[5];
  } gameGenie;

  function<uint16 (uint22 address)> read;
  function<void (uint22 address, uint16 data)> write;

  unique_pointer<Cartridge> slot;
  const uint depth = 0;
};

extern Cartridge cartridge;
