#ifndef GB_HPP
namespace GameBoy {
#endif

struct ID {
  enum : uint {
    System,
    GameBoy,
    SuperGameBoy,
    GameBoyColor,
  };

  enum : uint {
    SystemManifest,
    GameBoyBootROM,
    SuperGameBoyBootROM,
    GameBoyColorBootROM,

    Manifest,
    ROM,
    RAM,
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

  //Super Game Boy bindings
  struct Hook {
    virtual auto lcdScanline() -> void {}
    virtual auto lcdOutput(uint2 color) -> void {}
    virtual auto joypWrite(bool p15, bool p14) -> void {}
  };
  Hook* hook = nullptr;

  auto lcdScanline() -> void;
  auto lcdOutput(uint2 color) -> void;
  auto joypWrite(bool p15, bool p14) -> void;

private:
  vector<Device> device;
};

extern Interface* interface;

#ifndef GB_HPP
}
#endif
