#if defined(CORE_GBA)

namespace GameBoyAdvance {

struct ID {
  enum : uint {
    System,
    GameBoyAdvance,
  };

  struct Port { enum : uint {
    Hardware,
  };};

  struct Device { enum : uint {
    Controls,
  };};
};

struct Interface : Emulator::Interface {
  auto information() -> Information override;

  auto display() -> Display override;
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

  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct Settings {
  bool blurEmulation = true;
  bool colorEmulation = true;
  bool rotateLeft = false;
};

extern Settings settings;

}

#endif
