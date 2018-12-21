#if defined(CORE_MS)

namespace MasterSystem {

struct ID {
  enum : uint {
    System,
    SG1000,
    SC3000,
    MasterSystem,
    GameGear,
  };

  struct Port { enum : uint {
    Hardware,
    Controller1,
    Controller2,
  };};

  struct Device { enum : uint {
    None,
    SG1000Controls,
    SC3000Controls,
    MasterSystemControls,
    GameGearControls,
    Gamepad,
  };};
};

struct Interface : Emulator::Interface {
  auto loaded() -> bool override;
  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto save() -> void override;
  auto unload() -> void override;

  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>& list) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct SG1000Interface : Interface {
  auto information() -> Information override;

  auto displays() -> vector<Display> override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
};

struct SC3000Interface : Interface {
  auto information() -> Information override;

  auto displays() -> vector<Display> override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
};

struct MasterSystemInterface : Interface {
  auto information() -> Information override;

  auto displays() -> vector<Display> override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
};

struct GameGearInterface : Interface {
  auto information() -> Information override;

  auto displays() -> vector<Display> override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;
};

struct Settings {
  uint controllerPort1 = ID::Device::Gamepad;
  uint controllerPort2 = ID::Device::Gamepad;
};

extern Settings settings;

}

#endif
