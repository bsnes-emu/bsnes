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
    ControlPad,
    FightingPad,
  };};
};

struct Interface : Emulator::Interface {
  using Emulator::Interface::load;

  auto information() -> Information override;

  auto manifest() -> string override;
  auto title() -> string override;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto loaded() -> bool override;
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

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheatSet(const string_vector& list) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct Settings {
  uint controllerPort1 = ID::Device::ControlPad;
  uint controllerPort2 = ID::Device::ControlPad;
  uint extensionPort = ID::Device::None;
};

extern Settings settings;

}
