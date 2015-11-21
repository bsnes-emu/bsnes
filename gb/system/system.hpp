class Interface;

enum class Input : uint {
  Up, Down, Left, Right, B, A, Select, Start,
};

struct System {
  enum class Revision : uint {
    GameBoy,
    SuperGameBoy,
    GameBoyColor,
  } revision;

  System();

  inline auto dmg() const { return revision == Revision::GameBoy; }
  inline auto sgb() const { return revision == Revision::SuperGameBoy; }
  inline auto cgb() const { return revision == Revision::GameBoyColor; }

  auto run() -> void;
  auto runtosave() -> void;
  auto runthreadtosave() -> void;

  auto init() -> void;
  auto load(Revision) -> void;
  auto power() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serialize_all(serializer&) -> void;
  auto serialize_init() -> void;

  struct BootROM {
    uint8 dmg[ 256];
    uint8 sgb[ 256];
    uint8 cgb[2048];
  } bootROM;

  struct Information {
    string manifest;
  } information;

  uint clocks_executed = 0;
  uint serialize_size = 0;
};

#include <gb/interface/interface.hpp>

extern System system;
