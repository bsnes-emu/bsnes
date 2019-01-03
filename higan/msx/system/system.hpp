struct System {
  enum class Model : uint { MSX, MSX2, MSX2Plus, MSXTurboR };
  enum class Region : uint { NTSC, PAL };

  auto loaded() const -> bool { return information.loaded; }
  auto model() const -> Model { return information.model; }
  auto region() const -> Region { return information.region; }
  auto colorburst() const -> double { return information.colorburst; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;

  auto load(Emulator::Interface*, Model) -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

  Emulator::Memory::Readable<uint8> bios;

private:
  Emulator::Interface* interface = nullptr;

  struct Information {
    bool loaded = false;
    Model model = Model::MSX;
    Region region = Region::NTSC;
    double colorburst = Emulator::Constants::Colorburst::NTSC;
    string manifest;
    uint serializeSize = 0;
  } information;
};

extern System system;

auto Model::MSX() -> bool { return system.model() == System::Model::MSX; }
auto Model::MSX2() -> bool { return system.model() == System::Model::MSX2; }
auto Model::MSX2Plus() -> bool { return system.model() == System::Model::MSX2Plus; }
auto Model::MSXTurboR() -> bool { return system.model() == System::Model::MSXTurboR; }

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
