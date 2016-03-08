enum class Keypad : uint {
  Y1, Y2, Y3, Y4,
  X1, X2, X3, X4,
  B, A, Start,
};

struct System : IO {
  enum class Model : uint {
    WonderSwan,       //SW-001  (ASWAN)
    WonderSwanColor,  //WSC-001 (SPHINX)
    SwanCrystal,      //SCT-001 (SPHINX2)
  };

  auto loaded() const -> bool;
  auto model() const -> Model;
  auto color() const -> bool;
  auto planar() const -> bool;
  auto packed() const -> bool;

  auto init() -> void;
  auto term() -> void;
  auto load(Model) -> void;
  auto unload() -> void;
  auto power() -> void;
  auto run() -> void;

  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  struct Information {
    string manifest;
  } information;

  EEPROM eeprom;

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
};

extern System system;
