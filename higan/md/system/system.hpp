struct System {
  auto loaded() const -> bool { return information.loaded; }
  auto colorburst() const -> double { return information.colorburst; }

  auto run() -> void;

  auto load(Emulator::Interface*) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

private:
  Emulator::Interface* interface = nullptr;

  struct Information {
    bool loaded = false;
    string manifest;
    double colorburst = 0.0;
  } information;
};

struct Peripherals {
  auto unload() -> void;
  auto reset() -> void;
  auto connect(uint port, uint device) -> void;

  Controller* controllerPort1 = nullptr;
  Controller* controllerPort2 = nullptr;
  Controller* extensionPort = nullptr;
};

extern System system;
extern Peripherals peripherals;
