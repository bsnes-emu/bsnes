struct System {
  enum class Region : uint {
    NTSCJ,
    NTSCU,
    PAL,
  };

  auto loaded() const -> bool { return information.loaded; }
  auto region() const -> Region { return information.region; }
  auto colorburst() const -> double { return information.colorburst; }

  auto run() -> void;

  auto load(Emulator::Interface*, maybe<Region> = nothing) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

private:
  Emulator::Interface* interface = nullptr;

  struct Information {
    bool loaded = false;
    Region region = Region::NTSCJ;
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

auto Region::NTSCJ() -> bool { return system.region() == System::Region::NTSCJ; }
auto Region::NTSCU() -> bool { return system.region() == System::Region::NTSCU; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
