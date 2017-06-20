struct System {
  enum class Region : uint { NTSC, PAL };

  inline auto loaded() const -> bool { return information.loaded; }
  inline auto region() const -> Region { return information.region; }
  inline auto colorburst() const -> double { return information.colorburst; }

  auto run() -> void;
  auto runToSave() -> void;

  auto init() -> void;
  auto term() -> void;
  auto load(Emulator::Interface*) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  //video.cpp
  auto configureVideoPalette() -> void;
  auto configureVideoEffects() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

private:
  Emulator::Interface* interface = nullptr;

  struct Information {
    string manifest;
    bool loaded = false;
    Region region = Region::NTSC;
    double colorburst = Emulator::Constants::Colorburst::NTSC;
  } information;

  uint serializeSize = 0;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

  friend class Cartridge;
};

struct Peripherals {
  auto unload() -> void;
  auto reset() -> void;
  auto connect(uint port, uint device) -> void;

  Controller* controllerPort1 = nullptr;
  Controller* controllerPort2 = nullptr;
  Expansion* expansionPort = nullptr;
};

struct Random {
  auto seed(uint seed) -> void;
  auto operator()(uint result) -> uint;
  auto serialize(serializer& s) -> void;

private:
  uint iter = 0;
};

extern System system;
extern Peripherals peripherals;
extern Random random;

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
