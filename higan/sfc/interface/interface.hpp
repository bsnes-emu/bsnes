#if defined(CORE_SFC)

namespace SuperFamicom {

struct ID {
  enum : uint {
    System,
    SuperFamicom,
    GameBoy,
    BSMemory,
    SufamiTurboA,
    SufamiTurboB,
  };

  struct Port { enum : uint {
    Controller1,
    Controller2,
    Expansion,
  };};

  struct Device { enum : uint {
    None,
    Gamepad,
    Mouse,
    SuperMultitap,
    SuperScope,
    Justifier,
    Justifiers,

    Satellaview,
    S21FX,
  };};
};

struct Interface : Emulator::Interface {
  auto information() -> Information;

  auto displays() -> vector<Display> override;
  auto color(uint32 color) -> uint64 override;

  auto loaded() -> bool override;
  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto load() -> bool override;
  auto save() -> void override;
  auto unload() -> void override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto reset() -> void override;
  auto run() -> void override;

  auto rtc() -> bool override;
  auto synchronize(uint64 timestamp) -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>&) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct Settings {
  bool fastPPU = false;
  bool fastPPUNoSpriteLimit = false;
  bool fastPPUHiresMode7 = false;
  bool fastDSP = false;

  bool blurEmulation = true;
  bool colorEmulation = true;
  bool scanlineEmulation = true;

  uint controllerPort1 = ID::Device::Gamepad;
  uint controllerPort2 = ID::Device::Gamepad;
  uint expansionPort = ID::Device::None;
  bool random = true;
};

extern Settings settings;

}

#endif
