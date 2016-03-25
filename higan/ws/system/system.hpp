struct System : IO {
  auto loaded() const -> bool;
  auto model() const -> Model;
  auto orientation() const -> bool;
  auto color() const -> bool;
  auto planar() const -> bool;
  auto packed() const -> bool;
  auto depth() const -> bool;

  auto init() -> void;
  auto term() -> void;
  auto load(Model) -> void;
  auto unload() -> void;
  auto power() -> void;
  auto run() -> void;
  auto runToSave() -> void;
  auto pollKeypad() -> void;

  //io.cpp
  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

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

  struct Registers {
    //$0060  DISP_MODE
    uint1 depth;
    uint1 color;
    uint1 format;
    uint5 unknown;
  } r;

privileged:
  bool _loaded = false;
  Model _model = Model::WonderSwan;
  bool _orientation = 0;  //0 = horizontal, 1 = vertical
  uint _serializeSize = 0;
};

extern System system;
