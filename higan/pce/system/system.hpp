struct System {
  auto loaded() const -> bool { return information.loaded; }
  auto colorburst() const -> double { return information.colorburst; }

  auto run() -> void;

  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;

private:
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

  Controller* controllerPort = nullptr;
};

extern System system;
extern Peripherals peripherals;
