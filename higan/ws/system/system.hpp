enum class Keypad : uint {
  Y1, Y2, Y3, Y4,
  X1, X2, X3, X4,
  B, A, Start,
};

struct System : IO {
  enum class Revision : uint {
    WonderSwan,       //SW-001  (ASWAN)
    WonderSwanColor,  //WSC-001 (SPHINX)
    SwanCrystal,      //SCT-001 (SPHINX2)
  };

  auto loaded() const -> bool;
  auto revision() const -> Revision;

  auto init() -> void;
  auto term() -> void;
  auto load(Revision) -> void;
  auto unload() -> void;
  auto power() -> void;
  auto run() -> void;

  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  struct Information {
    string manifest;
  } information;

  EEPROM eeprom;

privileged:
  bool _loaded = false;
  Revision _revision = Revision::WonderSwan;
};

extern System system;
