struct System {
  enum class Model : uint { MasterSystem, GameGear };
  enum class Region : uint { NTSC, PAL };

  auto loaded() const -> bool { return information.loaded; }
  auto model() const -> Model { return information.model; }
  auto region() const -> Region { return information.region; }
  auto colorburst() const -> double { return information.colorburst; }

  auto run() -> void;
  auto runToSave() -> void;

  auto load(Emulator::Interface* interface, Model model) -> bool;
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
    bool loaded = false;
    Model model = Model::MasterSystem;
    Region region = Region::NTSC;
    double colorburst = Emulator::Constants::Colorburst::NTSC;
    string manifest;
    uint serializeSize = 0;
  } information;
};

struct Peripherals {
  auto unload() -> void;
  auto reset() -> void;
  auto connect(uint port, uint device) -> void;

  Controller* controllerPort1 = nullptr;
  Controller* controllerPort2 = nullptr;
};

extern System system;
extern Peripherals peripherals;

auto Model::MasterSystem() -> bool { return system.model() == System::Model::MasterSystem; }
auto Model::GameGear() -> bool { return system.model() == System::Model::GameGear; }

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
