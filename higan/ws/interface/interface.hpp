#if defined(CORE_WS)

namespace WonderSwan {

struct ID {
  enum : uint {
    System,
    WonderSwan,
    WonderSwanColor,
    PocketChallengeV2,
  };

  struct Port { enum : uint {
    Hardware,
  };};

  struct Device { enum : uint {
    Controls,
  };};
};

struct Interface : Emulator::Interface {
  auto displays() -> vector<Display> override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint ports) -> vector<Device> override;
  auto inputs(uint devices) -> vector<Input> override;

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

struct WonderSwanInterface : Interface {
  auto information() -> Information override;

  auto load() -> bool override;
};

struct WonderSwanColorInterface : Interface {
  auto information() -> Information override;

  auto load() -> bool override;
};

struct PocketChallengeV2Interface : Interface {
  auto information() -> Information override;

  auto load() -> bool override;
};

struct Settings {
  bool blurEmulation = true;
  bool colorEmulation = true;
  bool rotateLeft = false;
};

extern Settings settings;

}

#endif
