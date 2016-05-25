namespace Famicom {

struct ID {
  enum : uint {
    System,
    Famicom,
  };

  enum : uint {
    SystemManifest,

    Manifest,
    ProgramROM,
    ProgramRAM,
    CharacterROM,
    CharacterRAM,
  };

  enum : uint {
    Port1 = 1,
    Port2 = 2,
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
  auto sha256() -> string;
  auto group(uint id) -> uint;
  auto load(uint id) -> void;
  auto save() -> void;
  auto load(uint id, const stream& stream) -> void;
  auto save(uint id, const stream& stream) -> void;
  auto unload() -> void;

  auto power() -> void;
  auto reset() -> void;
  auto run() -> void;

  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto cheatSet(const lstring&) -> void;

  auto cap(const string& name) -> bool override;
  auto get(const string& name) -> any override;
  auto set(const string& name, const any& value) -> bool override;

private:
  vector<Device> devices;
};

struct Settings {
  bool colorEmulation = true;
  bool scanlineEmulation = true;
};

extern Interface* interface;
extern Settings settings;

}
