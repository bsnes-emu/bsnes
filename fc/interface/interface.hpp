#ifndef FC_HPP
namespace Famicom {
#endif

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

  auto title() -> string;
  auto videoFrequency() -> double;
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

  auto paletteUpdate(PaletteMode mode) -> void;

private:
  vector<Device> device;
};

extern Interface* interface;

#ifndef FC_HPP
}
#endif
