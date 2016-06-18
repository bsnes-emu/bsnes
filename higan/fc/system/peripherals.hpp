struct Port { enum : uint {
  Controller1,
  Controller2,
  Expansion,
};};

struct Device { enum : uint {
  None,

  //controller port peripherals
  Gamepad,

  //expansion port peripherals
};};

struct Peripherals {
  auto unload() -> void;
  auto reset() -> void;
  auto connect(uint port, uint device) -> void;

  Controller* controllerPort1 = nullptr;
  Controller* controllerPort2 = nullptr;
};

extern Peripherals peripherals;
