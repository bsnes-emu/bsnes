namespace PCEngine {

struct ID {
  enum : uint {
    System,
    PCEngine,
    SuperGrafx,
  };

  struct Port { enum : uint {
    Controller,
  };};

  struct Device { enum : uint {
    None,
    Gamepad,
  };};
};

struct Interface : Emulator::Interface {
  Interface();

  auto manifest() -> string override;
  auto title() -> string override;

  auto videoResolution() -> VideoResolution override;
  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;

  auto loaded() -> bool override;
  auto sha256() -> string override;
  auto save() -> void override;
  auto unload() -> void override;

  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheatSet(const string_vector&) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct PCEngineInterface : Interface {
  PCEngineInterface();

  auto load(uint id) -> bool override;
};

struct SuperGrafxInterface : Interface {
  SuperGrafxInterface();

  auto load(uint id) -> bool override;
};

struct Settings {
  uint controllerPort = 0;
};

extern Settings settings;

}
