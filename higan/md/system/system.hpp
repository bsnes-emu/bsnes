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
  auto runToSave() -> void;

  auto load(Emulator::Interface*, maybe<Region> = nothing) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

private:
  Emulator::Interface* interface = nullptr;

  struct Information {
    string manifest;
    bool loaded = false;
    Region region = Region::NTSCJ;
    double colorburst = Emulator::Constants::Colorburst::NTSC;
    uint serializeSize = 0;
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
