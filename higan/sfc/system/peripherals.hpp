struct Port { enum : uint {
  Controller1,
  Controller2,
  Expansion,
};};

struct Device { enum : uint {
  None,

  //controller port peripherals
  Gamepad,
  Multitap,
  Mouse,
  SuperScope,
  Justifier,
  Justifiers,

  //expansion port peripherals
  Satellaview,
  SuperDisc,
  S21FX,
};};

struct Peripherals {
  auto unload() -> void;
  auto reset() -> void;
  auto connect(uint port, uint id) -> void;

  Controller* controllerPort1 = nullptr;
  Controller* controllerPort2 = nullptr;
  Expansion* expansionPort = nullptr;
};

extern Peripherals peripherals;
