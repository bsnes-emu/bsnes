struct System : IO {
  enum class Model : uint { WonderSwan, WonderSwanColor, SwanCrystal };

  auto loaded() const -> bool { return _loaded; }
  auto model() const -> Model { return _model; }
  auto color() const -> bool { return r.color; }
  auto planar() const -> bool { return r.format == 0; }
  auto packed() const -> bool { return r.format == 1; }
  auto depth() const -> bool { return r.color && r.depth == 1; }

  auto init() -> void;
  auto term() -> void;
  auto load(Emulator::Interface*, Model) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto run() -> void;
  auto runToSave() -> void;
  auto pollKeypad() -> void;

  //io.cpp
  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  //video.cpp
  auto configureVideoPalette() -> void;
  auto configureVideoEffects() -> void;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

  struct Information {
    string manifest;
  } information;

  EEPROM eeprom;

  struct Keypad {
    bool y1, y2, y3, y4;
    bool x1, x2, x3, x4;
    bool b, a, start;
    bool rotate;
  } keypad;

private:
  Emulator::Interface* interface = nullptr;

  struct Registers {
    //$0060  DISP_MODE
    uint5 unknown;
    uint1 format;
    uint1 depth;
    uint1 color;
  } r;

  bool _loaded = false;
  Model _model = Model::WonderSwan;
  uint _serializeSize = 0;
};

extern System system;

auto Model::WonderSwan() -> bool { return system.model() == System::Model::WonderSwan; }
auto Model::WonderSwanColor() -> bool { return system.model() == System::Model::WonderSwanColor; }
auto Model::SwanCrystal() -> bool { return system.model() == System::Model::SwanCrystal; }
