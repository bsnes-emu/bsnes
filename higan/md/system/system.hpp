struct System {
  enum class Region : uint {
    NTSCJ,
    NTSCU,
    PAL,
  };

  auto loaded() const -> bool { return information.loaded; }
  auto region() const -> Region { return information.region; }
  auto frequency() const -> double { return information.frequency; }

  auto run() -> void;
  auto runToSave() -> void;

  auto load(Emulator::Interface*, maybe<Region> = nothing) -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power(bool reset) -> void;

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
    double frequency = Emulator::Constants::Colorburst::NTSC * 15.0;
    uint serializeSize = 0;
  } information;
};

extern System system;

auto Region::NTSCJ() -> bool { return system.region() == System::Region::NTSCJ; }
auto Region::NTSCU() -> bool { return system.region() == System::Region::NTSCU; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
