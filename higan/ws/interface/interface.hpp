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
  Interface();

  auto manifest() -> string override;
  auto title() -> string override;

  auto videoInformation() -> VideoInformation override;

  auto loaded() -> bool override;
  auto sha256() -> string override;
  auto save() -> void override;
  auto unload() -> void override;

  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheatSet(const string_vector&) -> void override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;
};

struct WonderSwanInterface : Interface {
  using Emulator::Interface::load;

  WonderSwanInterface();

  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;

  auto load(uint id) -> bool override;
};

struct WonderSwanColorInterface : Interface {
  using Emulator::Interface::load;

  WonderSwanColorInterface();

  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;

  auto load(uint id) -> bool override;
};

struct PocketChallengeV2Interface : Interface {
  using Emulator::Interface::load;

  PocketChallengeV2Interface();

  auto videoColors() -> uint32 override;
  auto videoColor(uint32 color) -> uint64 override;

  auto load(uint id) -> bool override;
};

struct Settings {
  bool blurEmulation = true;
  bool colorEmulation = true;
  bool rotateLeft = false;
};

extern Settings settings;

}
