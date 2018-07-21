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
  using Emulator::Interface::load;

  Interface();

  auto manifest() -> string override;
  auto title() -> string override;

  auto videoInformation() -> VideoInformation override;
  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;

  auto loaded() -> bool override;
  auto sha256() -> string override;
  auto load(uint id) -> bool override;
  auto save() -> void override;
  auto unload() -> void override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto reset() -> void override;
  auto run() -> void override;

  auto rtc() -> bool override;
  auto rtcSynchronize() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheatSet(const string_vector&) -> void override;

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
