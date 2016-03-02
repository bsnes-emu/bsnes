struct Device {
  enum class Port : uint {
    Controller1,
    Controller2,
    Expansion,
  };

  enum class ID : uint {
    None,

    //controller port devices
    Gamepad,
    Multitap,
    Mouse,
    SuperScope,
    Justifier,
    Justifiers,
    USART,

    //expansion port devices
    Satellaview,
    SuperDisc,
    eBoot,
  };

  Device();
  ~Device();

  auto connect(uint port, Device::ID id) -> void;

  Controller* controllerPort1 = nullptr;
  Controller* controllerPort2 = nullptr;
};

extern Device device;
