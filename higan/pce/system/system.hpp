struct System {
  enum class Model : uint { PCEngine, SuperGrafx };

  inline auto loaded() const -> bool { return information.loaded; }
  inline auto model() const -> Model { return information.model; }
  inline auto colorburst() const -> double { return information.colorburst; }

  auto run() -> void;

  auto load(Emulator::Interface*, Model) -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;

private:
  Emulator::Interface* interface = nullptr;

  struct Information {
    bool loaded = false;
    Model model = Model::PCEngine;
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

auto Model::PCEngine() -> bool { return system.model() == System::Model::PCEngine; }
auto Model::SuperGrafx() -> bool { return system.model() == System::Model::SuperGrafx; }
