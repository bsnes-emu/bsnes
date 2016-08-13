namespace WonderSwan {

struct ID {
  enum : uint {
    System,
    WonderSwan,
    WonderSwanColor,
  };

  struct Port { enum : uint {
    HardwareHorizontal,
    HardwareVertical,
  };};

  struct Device { enum : uint {
    Controls,
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
  auto videoColors() -> uint32;
  auto videoColor(uint32 color) -> uint64;

  auto audioFrequency() -> double override;

  auto loaded() -> bool override;
  auto sha256() -> string override;
  auto load(uint id) -> bool override;
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

struct Settings {
  bool blurEmulation = true;
  bool colorEmulation = true;
};

extern Interface* interface;
extern Settings settings;

}
