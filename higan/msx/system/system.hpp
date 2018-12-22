struct System {
  enum class Model : uint { MSX, MSX2, MSX2Plus, MSXTurboR };
  enum class Region : uint { NTSC, PAL };

  auto loaded() const -> bool { return information.loaded; }
  auto model() const -> Model { return information.model; }
  auto region() const -> Region { return information.region; }

  struct Information {
    string manifest;
    bool loaded = false;
    Model model = Model::MSX;
    Region region = Region::NTSC;
  } information;
};

extern System system;

auto Model::MSX() -> bool { return system.model() == System::Model::MSX; }
auto Model::MSX2() -> bool { return system.model() == System::Model::MSX2; }
auto Model::MSX2Plus() -> bool { return system.model() == System::Model::MSX2Plus; }
auto Model::MSXTurboR() -> bool { return system.model() == System::Model::MSXTurboR; }

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
