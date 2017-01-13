enum class Input : uint {
  Up, Down, Left, Right, B, A, Select, Start,
};

struct System {
  enum class Revision : uint {
    GameBoy,
    SuperGameBoy,
    GameBoyColor,
  };

  auto loaded() const -> bool { return _loaded; }
  auto revision() const -> Revision { return _revision; }
  auto clocksExecuted() const -> uint { return _clocksExecuted; }

  inline auto dmg() const { return _revision == Revision::GameBoy; }
  inline auto sgb() const { return _revision == Revision::SuperGameBoy; }
  inline auto cgb() const { return _revision == Revision::GameBoyColor; }

  auto run() -> void;
  auto runToSave() -> void;

  auto init() -> void;
  auto load(Emulator::Interface*, Revision) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  //video.cpp
  auto configureVideoPalette() -> void;
  auto configureVideoEffects() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

  Emulator::Interface* interface = nullptr;

  struct BootROM {
    uint8 dmg[ 256];
    uint8 sgb[ 256];
    uint8 cgb[2048];
  } bootROM;

  struct Information {
    string manifest;
  } information;

  bool _loaded = false;
  Revision _revision = Revision::GameBoy;
  uint _serializeSize = 0;
  uint _clocksExecuted = 0;
};

#include <gb/interface/interface.hpp>

extern System system;
