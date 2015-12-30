struct Device {
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
    eBoot,
  };

  Device();
  ~Device();

  auto connect(uint port, Device::ID id) -> void;

  Controller* controllerPort1 = nullptr;
  Controller* controllerPort2 = nullptr;
};

extern Device device;
