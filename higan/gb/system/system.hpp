enum class Input : uint {
  Up, Down, Left, Right, B, A, Select, Start,
};

struct System {
  enum class Model : uint {
    GameBoy,
    GameBoyColor,
    SuperGameBoy,
  };

  inline auto loaded() const -> bool { return _loaded; }
  inline auto model() const -> Model { return _model; }
  inline auto clocksExecuted() const -> uint { return _clocksExecuted; }

  auto run() -> void;
  auto runToSave() -> void;

  auto init() -> void;
  auto load(Emulator::Interface*, Model, maybe<uint> = nothing) -> bool;
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
  Model _model = Model::GameBoy;
  uint _serializeSize = 0;
  uint _clocksExecuted = 0;
};

#include <gb/interface/interface.hpp>

extern System system;

auto Model::GameBoy() -> bool { return system.model() == System::Model::GameBoy; }
auto Model::GameBoyColor() -> bool { return system.model() == System::Model::GameBoyColor; }
auto Model::SuperGameBoy() -> bool { return system.model() == System::Model::SuperGameBoy; }
