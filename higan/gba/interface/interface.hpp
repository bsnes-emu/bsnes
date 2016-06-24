namespace GameBoyAdvance {

struct ID {
  enum : uint {
    System,
    GameBoyAdvance,
  };

  enum : uint {
    Device = 1,
  };
};

struct Interface : Emulator::Interface {
  Interface();

  auto manifest() -> string;
  auto title() -> string;
  auto videoFrequency() -> double;
  auto videoColors() -> uint32;
  auto videoColor(uint32 color) -> uint64;
  auto audioFrequency() -> double;

  auto loaded() -> bool;
  auto load(uint id) -> void;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;
  auto reset() -> void;
  auto run() -> void;

  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;

private:
  vector<Device> devices;
};

struct Settings {
  bool blurEmulation = true;
  bool colorEmulation = true;
};

extern Interface* interface;
extern Settings settings;

}
