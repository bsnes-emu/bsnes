struct System {
  enum class Region : uint { NTSC, PAL };

  auto loaded() const -> bool { return information.loaded; }
  auto region() const -> Region { return information.region; }
  auto frequency() const -> double { return information.frequency; }

  auto run() -> void;
  auto runToSave() -> void;

  auto load(Emulator::Interface*) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto init() -> void;
  auto term() -> void;

  //video.cpp
  auto configureVideoPalette() -> void;
  auto configureVideoEffects() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

private:
  Emulator::Interface* interface = nullptr;

  struct Information {
    bool loaded = false;
    Region region = Region::NTSC;
    double frequency = Emulator::Constants::Colorburst::NTSC * 6.0;
    string manifest;
  } information;

  uint _serializeSize = 0;
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

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
