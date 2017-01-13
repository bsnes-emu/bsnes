namespace MegaDrive {

struct ID {
  enum : uint {
    System,
    MegaDrive,
  };

  struct Port { enum : uint {
    Controller1,
    Controller2,
    Extension,
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

  auto videoSize() -> VideoSize override;
  auto videoSize(uint width, uint height, bool arc) -> VideoSize override;
  auto videoFrequency() -> double override;
  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;

  auto audioFrequency() -> double override;

  auto loaded() -> bool override;
  auto load(uint id) -> bool override;
  auto save() -> void override;
  auto unload() -> void override;

  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto reset() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct Settings {
  uint controllerPort1 = 0;
  uint controllerPort2 = 0;
  uint extensionPort = 0;
};

extern Settings settings;

}
