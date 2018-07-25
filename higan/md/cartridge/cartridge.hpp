struct Cartridge {
  auto region() const -> string;
  auto hashes() const -> vector<string>;
  auto manifests() const -> vector<string>;
  auto titles() const -> vector<string>;

  struct Memory;
  auto load() -> bool;
  auto loadGame() -> bool;
  auto loadLockOn() -> bool;
  auto loadROM(Memory& rom, uint pathID, Markup::Node memory) -> bool;
  auto loadRAM(Memory& ram, uint pathID, Markup::Node memory) -> bool;
  auto save() -> void;
  auto saveRAM(Memory& ram, uint pathID, Markup::Node memory) -> bool;
  auto unload() -> void;
  auto power() -> void;

  function<uint16 (uint24 address)> read;
  function<void (uint24 address, uint16 data)> write;

  auto readIO(uint24 address) -> uint16;
  auto writeIO(uint24 address, uint16 data) -> void;

  auto readGame(uint24 address) -> uint16;
  auto writeGame(uint24 address, uint16 data) -> void;

  auto readLockOn(uint24 address) -> uint16;
  auto writeLockOn(uint24 address, uint16 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Memory {
    auto reset() -> void;

    uint16* data = nullptr;
    uint size = 0;
    uint mask = 0;
    uint bits = 0;
  };

  struct Game {
    uint pathID = 0;
    string region;
    string hash;
    string manifest;
    string title;

    Markup::Node document;
    Memory rom;
    Memory patch;
    Memory ram;
  } game;

  struct LockOn {
    uint pathID = 0;
    string hash;
    string manifest;
    string title;

    Markup::Node document;
    Memory rom;
    Memory ram;

    bool patch = false;
  } lockOn;

  uint1 ramEnable;
  uint1 ramWritable;
  uint6 bank[8];
};

extern Cartridge cartridge;
