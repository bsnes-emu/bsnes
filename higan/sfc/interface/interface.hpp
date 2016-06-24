namespace SuperFamicom {

struct ID {
  enum : uint {
    System,
    SuperFamicom,
    GameBoy,
    BSMemory,
    SufamiTurboA,
    SufamiTurboB,

    //deprecated
    SuperGameBoyManifest,
    SuperGameBoyBootROM,

    GameBoyManifest,
    GameBoyROM,
    GameBoyRAM,
  };

  enum : uint {
    //device ports (bitmask)
    ControllerPort1 = 1,
    ControllerPort2 = 2,
    ExpansionPort   = 4,
  };
};

struct Interface : Emulator::Interface {
  using Emulator::Interface::load;

  Interface();

  auto manifest() -> string;
  auto title() -> string;
  auto videoFrequency() -> double;
  auto videoColors() -> uint32;
  auto videoColor(uint32 color) -> uint64;
  auto audioFrequency() -> double;

  auto loaded() -> bool;
  auto sha256() -> string;
  auto load(uint id) -> void;
  auto save() -> void;
  auto unload() -> void;

  auto connect(uint port, uint device) -> void;
  auto power() -> void;
  auto reset() -> void;
  auto run() -> void;

  auto rtc() -> bool;
  auto rtcsync() -> void;

  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto cheatSet(const lstring&) -> void;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;

  vector<Device> devices;
};

struct Settings {
  bool blurEmulation = true;
  bool colorEmulation = true;
  bool scanlineEmulation = true;

  uint controllerPort1 = 0;
  uint controllerPort2 = 0;
  uint expansionPort = 0;
  bool random = true;
};

extern Interface* interface;
extern Settings settings;

}
