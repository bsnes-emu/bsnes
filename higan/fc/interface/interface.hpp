namespace Famicom {

struct ID {
  enum : uint {
    System,
    Famicom,
  };

  struct Port { enum : uint {
    Controller1,
    Controller2,
    Expansion,
  };};

  struct Device { enum : uint {
    None,
    Gamepad,
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

  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto reset() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheatSet(const string_vector&) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct Settings {
  bool colorEmulation = true;
  bool scanlineEmulation = true;

  uint controllerPort1 = 0;
  uint controllerPort2 = 0;
  uint expansionPort = 0;
};

extern Settings settings;

}
