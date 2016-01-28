namespace WonderSwan {

struct ID {
  enum : uint {
    System,
    WonderSwan,
    WonderSwanColor,
  };

  enum : uint {
    SystemManifest,

    Manifest,
    ROM,
    RAM,
  };

  enum : uint {
    DeviceHorizontal = 1,
    DeviceVertical   = 2,
  };
};

struct Interface : Emulator::Interface {
  Interface();

  auto manifest() -> string override;
  auto title() -> string override;
  auto videoFrequency() -> double override;
  auto audioFrequency() -> double override;

  auto loaded() -> bool override;
  auto sha256() -> string override;
  auto group(uint id) -> uint override;
  auto load(uint id) -> void override;
  auto save() -> void override;
  auto load(uint id, const stream& stream) -> void override;
  auto save(uint id, const stream& stream) -> void override;
  auto unload() -> void override;

  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;

private:
  vector<Device> device;
};

struct Settings {
};

extern Interface* interface;
extern Settings settings;

}
