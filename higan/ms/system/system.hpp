struct System {
  enum class Model : uint { ColecoVision, SG1000, SC3000, MasterSystem, GameGear };
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

  uint8 bios[0x2000];

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

extern System system;

auto Model::ColecoVision() -> bool { return system.model() == System::Model::ColecoVision; }
auto Model::SG1000() -> bool { return system.model() == System::Model::SG1000; }
auto Model::SC3000() -> bool { return system.model() == System::Model::SC3000; }
auto Model::MasterSystem() -> bool { return system.model() == System::Model::MasterSystem; }
auto Model::GameGear() -> bool { return system.model() == System::Model::GameGear; }

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
